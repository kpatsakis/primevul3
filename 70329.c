uint32_t GPMF_RawDataSize(GPMF_stream *ms)
{
	if (ms && ms->pos+1 < ms->buffer_size_longs)
	{
		uint32_t size = GPMF_DATA_PACKEDSIZE(ms->buffer[ms->pos + 1]);
		if (GPMF_OK != IsValidSize(ms, size >> 2)) return 0;

		return size;
	}
	return 0;
}
