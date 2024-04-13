GPMF_ERR GPMF_FindNext(GPMF_stream *ms, uint32_t fourcc, GPMF_LEVELS recurse)
{
	GPMF_stream prevstate;

	if (ms)
	{
		memcpy(&prevstate, ms, sizeof(GPMF_stream));

		if (ms->pos < ms->buffer_size_longs)
		{
			while (0 == GPMF_Next(ms, recurse))
			{
				if (ms->buffer[ms->pos] == fourcc)
				{
					return GPMF_OK; //found match
				}
			}

			memcpy(ms, &prevstate, sizeof(GPMF_stream));
			return GPMF_ERROR_FIND;
		}
	}
	return GPMF_ERROR_FIND;
}
