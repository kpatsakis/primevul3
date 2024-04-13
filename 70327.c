uint32_t GPMF_PayloadSampleCount(GPMF_stream *ms)
{
	uint32_t count = 0;
	if (ms)
	{
		uint32_t fourcc = GPMF_Key(ms);

		GPMF_stream find_stream;
		GPMF_CopyState(ms, &find_stream);

		if (GPMF_OK == GPMF_FindNext(&find_stream, fourcc, GPMF_CURRENT_LEVEL)) // Count the instances, not the repeats
		{
			count=2;
			while (GPMF_OK == GPMF_FindNext(&find_stream, fourcc, GPMF_CURRENT_LEVEL))
			{
				count++;
			} 
		}
		else
		{
			count = GPMF_Repeat(ms);
		}
	}
	return count;
}
