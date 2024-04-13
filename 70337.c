uint32_t GPMF_Type(GPMF_stream *ms)
{
	if (ms && ms->pos+1 < ms->buffer_size_longs)
	{
		uint32_t type = GPMF_SAMPLE_TYPE(ms->buffer[ms->pos+1]);
		return type;
	}
	return 0;
}
