static GF_Err FFD_DisconnectChannel(GF_InputService *plug, LPNETCHANNEL channel)
{
	GF_Err e;
	FFDemux *ffd = (FFDemux*)plug->priv;

	e = GF_STREAM_NOT_FOUND;
	if (ffd->audio_ch == channel) {
		e = GF_OK;
		ffd->audio_ch = NULL;
		ffd->audio_run = GF_FALSE;
	}
	else if (ffd->video_ch == channel) {
		e = GF_OK;
		ffd->video_ch = NULL;
		ffd->video_run = GF_FALSE;
	}
	gf_service_disconnect_ack(ffd->service, channel, e);
	return GF_OK;
}
