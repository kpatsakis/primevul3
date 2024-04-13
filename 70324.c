GPMF_ERR GPMF_Init(GPMF_stream *ms, uint32_t *buffer, int datasize)
{
	if(ms)
	{
		ms->buffer = buffer;
		ms->buffer_size_longs = datasize >>2;

		GPMF_ResetState(ms);

		return GPMF_OK;
	}
	
	return GPMF_ERROR_MEMORY;
}
