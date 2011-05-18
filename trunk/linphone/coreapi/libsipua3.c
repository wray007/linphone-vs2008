#include "linphonecore.h"
#include "private.h"


int linphone_core_hold(LinphoneCore *lc, bool_t val, int *cid){
	char text[256];
	int cid_used;
	LinphoneCall *lcall;
	
	if(val)
	{	
		if(cid == NULL)
		{
			if(!linphone_core_is_in_conversation(lc))
			{
				lc->vtable.display_warning(lc,_("we are not in conversation cannot hold on"));
				return -4;
			}
			cid_used = linphone_core_get_call_cid(linphone_core_get_current_call(lc));
		}
		else
		{
			cid_used = *cid;
		}
	}
	else
	{	//hold off
		if(linphone_core_is_in_conversation(lc))
		{
			lc->vtable.display_warning(lc,_("cannot hold off we are already in communication"));
			return -3;
		}
		if(cid == NULL)
		{
			lc->vtable.display_warning(lc,_("cannot hold a call without the cid specify"));
			return -2;
		}
		cid_used = *cid;
	}
	lcall = linphone_core_get_call_by_cid(lc,cid_used);
	if(lcall == NULL)
	{
		snprintf(text,sizeof(text),"the call with cid <%d> do no exist",cid_used);
		lc->vtable.display_status(lc,text);
		return -1;
	}
	if(val==HOLD_ON)
		snprintf(text,sizeof(text),"hold on the call with cid <%d>",cid_used);
	else
		snprintf(text,sizeof(text),"hold off the call with cid <%d>",cid_used);
	lc->vtable.display_status(lc,text);
	linphone_core_send_hold(lc,val,&cid_used);
	if(val==HOLD_ON)
		linphone_core_stop_media_streams(lc,lcall);
	else
		linphone_core_start_media_streams(lc,lcall);
	
	return 0;
}

/**
 * Send a re-Invite used to hold the current call
 *
 * @ingroup call_control
 * @param lc the LinphoneCore object
 * @param url the destination of the call (sip address).
**/
int linphone_core_send_hold(LinphoneCore *lc, int hold, int *cid)
{
	int err=0;
	char *sdpmesg=NULL;
	osip_message_t *reinvite=NULL;
	char *real_url=NULL;
	char *route=NULL;
	LinphoneCall *call;
	sdp_context_t *ctx=NULL;
	LinphoneProxyConfig *dest_proxy=NULL;
	char tmp_cseq_number[5];
	
	if(cid ==NULL)
	{
		ms_error("cannot send hold without cid");
		return -1;
	}
	call=linphone_core_get_call_by_cid(lc,*cid);
	if (call==NULL){
		ms_error("cannot find the call with cid <%d>",*cid);
		return -1;
	}
	err=eXosip_call_build_initial_invite( &reinvite,linphone_address_as_string(call->log->to),linphone_address_as_string(call->log->from),route,"Phone call Hold");
	
	if (err<0){
		ms_warning("Could not build initial invite");
		goto end;
	}
	/* This is probably useless for other messages */
	osip_message_set_allow(reinvite, "INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO");
	//wrong cseq sent
	//osip_cseq_free(reinvite->cseq);
	call->nb_cseq;
	snprintf(tmp_cseq_number,sizeof(tmp_cseq_number),"%d",(atoi(osip_cseq_get_number(reinvite->cseq))(call->nb_cseq)));
	osip_cseq_set_number(reinvite->cseq, osip_strdup(tmp_cseq_number));
	/* setting the previous tag and the previous call-ID from the first invite*/
	ms_message("setting the previous tag %s",call->tag_transaction_value);
	ms_message("setting the previous call-id %s",call->the_callid);
	{
		osip_uri_param_t *p_tag;
		osip_from_get_tag(reinvite->from,&p_tag);
		osip_generic_param_set_value(p_tag,osip_strdup(call->tag_transaction_value));
		osip_call_id_set_number(reinvite->call_id,osip_strdup(call->the_callid));
	}
	if (lp_config_get_int(lc->config,"sip","use_session_timers",0)==1){
		osip_message_set_header(reinvite, "Session-expires", "200");
		osip_message_set_supported(reinvite, "timer");
	}
	/*try to be best-effort in giving real local or routable contact address,
	except when the user choosed to override the ipaddress */
	if (linphone_core_get_firewall_policy(lc)!=LINPHONE_POLICY_USE_NAT_ADDRESS)
		fix_contact(lc,reinvite,call->localip,dest_proxy);
	/* make sdp message */
	
	if (!lc->sip_conf.sdp_200_ack){
		ctx=call->sdpctx;
		ctx->holdon = hold;
		sdpmesg=sdp_context_get_offer(ctx);
		linphone_set_sdp(reinvite,sdpmesg);
	}
	eXosip_lock();
	err=eXosip_call_send_request(call->did, reinvite);
	eXosip_unlock();
end:
	if (real_url!=NULL) ms_free(real_url);
	if (route!=NULL) ms_free(route);
	return (err<0) ? -1 : 0;
}