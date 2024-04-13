uint32_t GPMF_Repeat(GPMF_stream *ms)
{
	if (ms && ms->pos+1 < ms->buffer_size_longs)
	{
		uint32_t repeat = GPMF_SAMPLES(ms->buffer[ms->pos + 1]);
		return repeat;
	}
	return 0;
}
