uint32_t GPMF_NestLevel(GPMF_stream *ms)
{
	if (ms)
	{
		return ms->nest_level;
	}
	return 0;
}
