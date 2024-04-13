void *New_FFMPEG_Demux()
{
	GF_InputService *ffd;
	FFDemux *priv;
	GF_SAFEALLOC(ffd, GF_InputService);
	if (!ffd) return NULL;
	GF_SAFEALLOC(priv, FFDemux);
	if (!priv) {
		gf_free(ffd);
		return NULL;
	}
	GF_LOG(GF_LOG_INFO, GF_LOG_CONTAINER, ("[FFMPEG Demuxer] Registering all ffmpeg plugins...\n") );
	/* register all codecs, demux and protocols */
	av_register_all();
	avformat_network_init();
	GF_LOG(GF_LOG_DEBUG, GF_LOG_CONTAINER, ("[FFMPEG Demuxer] Registering all ffmpeg plugins DONE.\n") );

	ffd->RegisterMimeTypes = FFD_RegisterMimeTypes;
	ffd->CanHandleURL = FFD_CanHandleURL;
	ffd->CloseService = FFD_CloseService;
	ffd->ConnectChannel = FFD_ConnectChannel;
	ffd->ConnectService = FFD_ConnectService;
	ffd->DisconnectChannel = FFD_DisconnectChannel;
	ffd->GetServiceDescriptor = FFD_GetServiceDesc;
	ffd->ServiceCommand = FFD_ServiceCommand;

	ffd->CanHandleURLInService = FFD_CanHandleURLInService;

	priv->thread = gf_th_new("FFMPEG Demux");
	priv->mx = gf_mx_new("FFMPEG Demux");
	if (!priv->thread || !priv->mx) {
		if (priv->thread) gf_th_del(priv->thread);
		if (priv->mx) gf_mx_del(priv->mx);
		gf_free(priv);
		return NULL;
	}

	GF_REGISTER_MODULE_INTERFACE(ffd, GF_NET_CLIENT_INTERFACE, "FFMPEG Demuxer", "gpac distribution");
	ffd->priv = priv;
	return ffd;
}
