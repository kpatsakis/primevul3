uint32_t GPMF_StructSize(GPMF_stream *ms)
{
	if (ms && ms->pos+1 < ms->buffer_size_longs)
	{
		uint32_t ssize = GPMF_SAMPLE_SIZE(ms->buffer[ms->pos + 1]);
		uint32_t size = (GPMF_DATA_SIZE(ms->buffer[ms->pos + 1]) >> 2);

		if (GPMF_OK != IsValidSize(ms, size)) return 0; // as the structure is corrupted. i.e. GPMF_ERROR_BAD_STRUCTURE;

		return ssize;
	}
	return 0;
}
