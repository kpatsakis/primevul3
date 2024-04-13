void Delete_FFMPEG_Demux(void *ifce)
{
	FFDemux *ffd;
	GF_InputService *ptr = (GF_InputService *)ifce;
	if (!ptr)
		return;
	ffd = (FFDemux*)ptr->priv;
	if (ffd) {
		if (ffd->thread)
			gf_th_del(ffd->thread);
		ffd->thread = NULL;
		if (ffd->mx)
			gf_mx_del(ffd->mx);

#ifndef USE_PRE_0_7
		if (ffd->options) av_dict_free(&ffd->options);
#endif
		ffd->mx = NULL;
		gf_free(ffd);
		ptr->priv = NULL;
	}
	gf_free(ptr);
}
