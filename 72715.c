void ffd_parse_options(FFDemux *ffd, const char *url)
{
#ifdef USE_AVFORMAT_OPEN_INPUT
	int res;
	char *frag = (char*) strchr(url, '#');
	if (frag) frag = frag+1;

	if (ffd->options) return;

	while (frag) {
		char *mid, *sep = strchr(frag, ':');
		if (sep) sep[0] = 0;
		mid = strchr(frag, '=');
		if (mid) {
			mid[0] = 0;
			res = av_dict_set(&ffd->options, frag, mid+1, 0);
			if (res<0) {
				GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("[FFMPEG Demuxer] Failed to set option %s:%s\n", frag, mid+1) );
			}
			mid[0] = '=';
		}
		if (!sep) break;
		sep[0] = ':';
		frag = sep+1;
	}
#endif
}
