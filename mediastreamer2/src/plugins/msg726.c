/*
mediastreamer2 library - modular sound and video processing and streaming
Copyright (C) 2006  Simon MORLAT (simon.morlat@linphone.org)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifdef WIN32
#include <malloc.h>  // for alloca
#pragma comment(lib, "libg726.lib")
#endif

#include "mediastreamer2/msfilter.h"

#include "g726.h"

// struct g726_state_s;
// typedef struct g726_state_s g726_state_t;
// 
// g726_state_t *g726_init(g726_state_t *s, int bit_rate, int ext_coding, int packing);
// int g726_release(g726_state_t *s);
// int g726_decode(g726_state_t *s,
//                 int16_t amp[],
//                 const uint8_t g726_data[],
//                 int g726_bytes);
// 
// int g726_encode(g726_state_t *s,
//                 uint8_t g726_data[],
//                 const int16_t amp[],
//                 int len);


typedef struct EncState{
	g726_state_t *state;
	uint32_t ts;
	int   ptime;
	MSBufferizer *bufferizer;
} EncState;

static void enc_init(MSFilter *f){
	EncState *s=(EncState *)ms_new(EncState,1);
	s->state = g726_init(0,  32000, 0, 2);
	s->ts=0;
	s->bufferizer=ms_bufferizer_new();
	s->ptime = 20;
	f->data=s;
}

static void enc_uninit(MSFilter *f){
	EncState *s=(EncState*)f->data;
	g726_release(s->state);
	ms_bufferizer_destroy(s->bufferizer);
	ms_free(s);
	f->data = 0;
}


static void enc_process(MSFilter *f){
	EncState *s=(EncState*)f->data;
	mblk_t *im;
	int nbytes;
	uint8_t *buf;
	int frame_per_packet=1;
	int chunksize;
	
	if (s->ptime>=10)
	{
		frame_per_packet = s->ptime/10;
	}

	if (frame_per_packet<=0)
		frame_per_packet=1;

	
	nbytes=80*2;  /*  10 Msec at 8KHZ  = 160 bytes of data */
	buf=(uint8_t*)alloca(nbytes*frame_per_packet);

	while((im=ms_queue_get(f->inputs[0]))!=NULL){
		ms_bufferizer_put(s->bufferizer,im);
	}
	
	chunksize = nbytes*frame_per_packet;
	while(ms_bufferizer_read(s->bufferizer,buf, chunksize)==chunksize){
		mblk_t *om=allocb(nbytes*frame_per_packet,0);//too large...
		int k;
		
		k = g726_encode(s->state, om->b_wptr, (int16_t *)buf, chunksize/2);		
		om->b_wptr+=k;
		mblk_set_timestamp_info(om,s->ts);		
		ms_queue_put(f->outputs[0],om);
		s->ts += chunksize/2; 

	}
}


static int enc_add_attr(MSFilter *f, void *arg){
	const char *fmtp=(const char *)arg;
	EncState *s=(EncState*)f->data;
	if (strstr(fmtp,"ptime:")!=NULL){
	  s->ptime = atoi(fmtp+6);
	}
	return 0;
}


static MSFilterMethod enc_methods[]={
	{	MS_FILTER_ADD_ATTR		,	enc_add_attr},
	{	0				,	NULL		}
};


#ifdef _MSC_VER

MSFilterDesc ms_g726_enc_desc={
	MS_FILTER_PLUGIN_ID,
	"MSG726Enc",
	"The G726 codec",
	MS_FILTER_ENCODER,
	"g726-32",
	1,
	1,
	enc_init,
	NULL,
	enc_process,
	NULL,
	enc_uninit,
	enc_methods
};

#else

MSFilterDesc ms_g726_enc_desc={
	.id=MS_FILTER_PLUGIN_ID,
	.name="MSG726Enc",
	.text="The G726 codec",
	.category=MS_FILTER_ENCODER,
	.enc_fmt="g726-32",
	.ninputs=1,
	.noutputs=1,
	.init=enc_init,
	.process=enc_process,
	.uninit=enc_uninit,
	.methods = enc_methods
};

#endif

typedef struct DecState {
	g726_state_t *state;
} DecState;



static void dec_init(MSFilter *f){
	DecState *s=(DecState *)ms_new(DecState,1);
	f->data=s;

	s->state = g726_init(0,  32000, 0, 2);
}

static void dec_uninit(MSFilter *f){
  DecState *s=(DecState *)f->data;
  g726_release(s->state);
  ms_free(f->data);
  f->data = 0;
}


static void dec_process(MSFilter *f){
	DecState *s=(DecState *)f->data;
	mblk_t *im;
	mblk_t *om;

	while((im=ms_queue_get(f->inputs[0]))!=NULL){
	        int payloadlen = im->b_wptr - im->b_rptr;
		int declen;
		om=allocb(payloadlen*4,0);
		if ((declen = g726_decode(s->state,(int16_t *)om->b_wptr, im->b_rptr, payloadlen))<0){
			ms_warning("g726_decode error!");
			freemsg(om);
		}else{
			om->b_wptr+= declen*2;
			ms_queue_put(f->outputs[0],om);
		}
		freemsg(im);
	}

}

#ifdef _MSC_VER

MSFilterDesc ms_g726_dec_desc={
	MS_FILTER_PLUGIN_ID,
	"MSG726Dec",
	"The G726 codec",
	MS_FILTER_DECODER,
	"g726-32",
	1,
	1,
	dec_init,
	NULL,
	dec_process,
	NULL,
	dec_uninit,
	NULL
};

#else

MSFilterDesc ms_g726_dec_desc={
	.id=MS_FILTER_PLUGIN_ID,
	.name="MSG726Dec",
	.text="The G726 codec",
	.category=MS_FILTER_DECODER,
	.enc_fmt="g726-32",
	.ninputs=1,
	.noutputs=1,
	.init=dec_init,
	.process=dec_process,
	.uninit=dec_uninit
};

#endif



#ifdef ENABLE_PLUGIN
void libmsg726_init(void){
	ms_filter_register(&ms_g726_dec_desc);
	ms_filter_register(&ms_g726_enc_desc);
}
#else
MS_FILTER_DESC_EXPORT(ms_g726_dec_desc)
MS_FILTER_DESC_EXPORT(ms_g726_enc_desc)
#endif // ENABLE_PLUGIN