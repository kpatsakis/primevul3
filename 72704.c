static Bool FFD_CanHandleURLInService(GF_InputService *plug, const char *url)
{
	char szURL[2048], *sep;
	FFDemux *ffd;
	const char *this_url;
	if (!plug || !url)
		return GF_FALSE;
	ffd = (FFDemux *)plug->priv;
	this_url = gf_service_get_url(ffd->service);
	if (!this_url)
		return GF_FALSE;

	strcpy(szURL, this_url);
	sep = strrchr(szURL, '#');
	if (sep) sep[0] = 0;

	if ((url[0] != '#') && strnicmp(szURL, url, sizeof(char)*strlen(szURL))) return GF_FALSE;
	sep = strrchr(url, '#');
	if (sep && !stricmp(sep, "#video") && (ffd->video_st>=0)) return GF_TRUE;
	if (sep && !stricmp(sep, "#audio") && (ffd->audio_st>=0)) return GF_TRUE;
	return GF_FALSE;
}
