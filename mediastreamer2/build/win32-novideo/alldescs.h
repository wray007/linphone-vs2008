#include "mediastreamer2/msfilter.h"

extern MSFilterDesc ms_alaw_dec_desc;
extern MSFilterDesc ms_alaw_enc_desc;
extern MSFilterDesc ms_ulaw_dec_desc;
extern MSFilterDesc ms_ulaw_enc_desc;
extern MSFilterDesc ms_file_player_desc;
extern MSFilterDesc ms_rtp_send_desc;
extern MSFilterDesc ms_rtp_recv_desc;
extern MSFilterDesc ms_dtmf_gen_desc;
extern MSFilterDesc ms_file_rec_desc;
extern MSFilterDesc ms_speex_dec_desc;
extern MSFilterDesc ms_speex_enc_desc;
extern MSFilterDesc ms_gsm_dec_desc;
extern MSFilterDesc ms_gsm_enc_desc;
extern MSFilterDesc ms_speex_ec_desc;
extern MSFilterDesc ms_tee_desc;
extern MSFilterDesc ms_conf_desc;
extern MSFilterDesc ms_bv16_dec_desc;
extern MSFilterDesc ms_bv16_enc_desc;
extern MSFilterDesc ms_bv32_dec_desc;
extern MSFilterDesc ms_bv32_enc_desc;
#if 0
extern MSFilterDesc ms_g722_dec_desc;
extern MSFilterDesc ms_g722_enc_desc;
#endif // 0
#ifndef _DEBUG
extern MSFilterDesc ms_g723_enc_desc;
extern MSFilterDesc ms_g723_dec_desc;
extern MSFilterDesc ms_g729_enc_desc;
extern MSFilterDesc ms_g729_dec_desc;
#endif // _DEBUG
extern MSFilterDesc ms_g726_dec_desc;
extern MSFilterDesc ms_g726_enc_desc;
extern MSFilterDesc ms_ilbc_enc_desc;
extern MSFilterDesc ms_ilbc_dec_desc;
//extern MSFilterDesc alsa_write_desc;
//extern MSFilterDesc alsa_read_desc;
//extern MSFilterDesc oss_read_desc;
//extern MSFilterDesc oss_write_desc;
//extern MSFilterDesc ms_arts_read_desc;
//extern MSFilterDesc ms_arts_write_desc;
//extern MSFilterDesc ms_v4l_desc;
//extern MSFilterDesc ms_sdl_out_desc;
extern MSFilterDesc ms_h263_enc_desc;
extern MSFilterDesc ms_h263_dec_desc;
extern MSFilterDesc ms_h264_dec_desc;
extern MSFilterDesc x264_enc_desc;
extern MSFilterDesc ms_join_desc;
extern MSFilterDesc ms_resample_desc;
extern MSFilterDesc ms_volume_desc;
extern MSFilterDesc ms_ice_desc;
extern MSFilterDesc ms_equalizer_desc;
extern MSFilterDesc ms_audio_mixer_desc;
extern MSFilterDesc ms_channel_adapter_desc;
extern MSFilterDesc ms_pix_conv_desc;
extern MSFilterDesc ms_size_conv_desc;
extern MSFilterDesc ms_dd_display_desc;
MSFilterDesc * ms_filter_descs[]={
&ms_alaw_dec_desc,
&ms_alaw_enc_desc,
&ms_ulaw_dec_desc,
&ms_ulaw_enc_desc,
&ms_file_player_desc,
&ms_rtp_send_desc,
&ms_rtp_recv_desc,
&ms_dtmf_gen_desc,
&ms_file_rec_desc,
&ms_speex_dec_desc,
&ms_speex_enc_desc,
&ms_gsm_dec_desc,
&ms_gsm_enc_desc,
&ms_bv16_dec_desc,
&ms_bv16_enc_desc,
&ms_bv32_dec_desc,
&ms_bv32_enc_desc,
#if 0
&ms_g722_dec_desc,
&ms_g722_enc_desc,
#endif // 0
#ifndef _DEBUG
&ms_g723_enc_desc,
&ms_g723_dec_desc,
&ms_g729_dec_desc,
&ms_g729_enc_desc,
#endif // _DEBUG
&ms_g726_dec_desc,
&ms_g726_enc_desc,
&ms_ilbc_enc_desc,
&ms_ilbc_dec_desc,
&ms_speex_ec_desc,
&ms_tee_desc,
&ms_conf_desc,
//&alsa_write_desc,
//&alsa_read_desc,
//&oss_read_desc,
//&oss_write_desc,
//&ms_arts_read_desc,
//&ms_arts_write_desc,
//&ms_v4l_desc,
//&ms_sdl_out_desc,
&ms_h263_enc_desc,
&ms_h263_dec_desc,
&ms_h264_dec_desc,
&x264_enc_desc,
&ms_join_desc,
#ifndef DISABLE_RESAMPLE
&ms_resample_desc,
#endif
&ms_volume_desc,
&ms_ice_desc,
&ms_equalizer_desc,
&ms_audio_mixer_desc,
&ms_channel_adapter_desc,
&ms_pix_conv_desc,
&ms_size_conv_desc,
&ms_dd_display_desc,
NULL
};

