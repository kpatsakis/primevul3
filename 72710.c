static GF_Err FFD_ServiceCommand(GF_InputService *plug, GF_NetworkCommand *com)
{
	FFDemux *ffd = (FFDemux*)plug->priv;


	if (com->command_type==GF_NET_SERVICE_HAS_AUDIO) {
		if (ffd->audio_st>=0) return GF_OK;
		return GF_NOT_SUPPORTED;
	}

	if (!com->base.on_channel) return GF_NOT_SUPPORTED;

	switch (com->command_type) {
	/*only BIFS/OD work in pull mode (cf ffmpeg_in.h)*/
	case GF_NET_CHAN_SET_PULL:
		return GF_NOT_SUPPORTED;
	case GF_NET_CHAN_INTERACTIVE:
		return ffd->seekable ? GF_OK : GF_NOT_SUPPORTED;
	case GF_NET_CHAN_BUFFER:
		return GF_OK;
	case GF_NET_CHAN_DURATION:
		if (ffd->ctx->duration == AV_NOPTS_VALUE)
			com->duration.duration = -1;
		else
			com->duration.duration = (Double) ffd->ctx->duration / AV_TIME_BASE;
		return GF_OK;
	/*fetch start time*/
	case GF_NET_CHAN_PLAY:
		if (com->play.speed<0) return GF_NOT_SUPPORTED;

		gf_mx_p(ffd->mx);
		ffd->seek_time = (com->play.start_range>=0) ? com->play.start_range : 0;

		if (ffd->audio_ch==com->base.on_channel) ffd->audio_run = GF_TRUE;
		else if (ffd->video_ch==com->base.on_channel) ffd->video_run = GF_TRUE;

		/*play on media stream, start thread*/
		if ((ffd->audio_ch==com->base.on_channel) || (ffd->video_ch==com->base.on_channel)) {
			if (ffd->is_running!=1) {
				ffd->is_running = 1;
				gf_th_run(ffd->thread, FFDemux_Run, ffd);
			}
		}
		gf_mx_v(ffd->mx);
		return GF_OK;
	case GF_NET_CHAN_STOP:
		if (ffd->audio_ch==com->base.on_channel) ffd->audio_run = GF_FALSE;
		else if (ffd->video_ch==com->base.on_channel) ffd->video_run = GF_FALSE;
		return GF_OK;
	/*note we don't handle PAUSE/RESUME/SET_SPEED, this is automatically handled by the demuxing thread
	through buffer occupancy queries*/

	default:
		return GF_OK;
	}

	return GF_OK;
}
