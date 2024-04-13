void *GPMF_RawData(GPMF_stream *ms)
{
	if (ms)
	{
		return (void *)&ms->buffer[ms->pos + 2];
	}
	return NULL;
}
