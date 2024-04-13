GPMF_ERR GPMF_CopyState(GPMF_stream *msrc, GPMF_stream *mdst)
{
	if (msrc && mdst)
	{
		memcpy(mdst, msrc, sizeof(GPMF_stream));
		return GPMF_OK;
	}
	return GPMF_ERROR_MEMORY;
}
