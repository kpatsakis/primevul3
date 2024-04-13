GPMF_ERR GPMF_FindPrev(GPMF_stream *ms, uint32_t fourcc, GPMF_LEVELS recurse)
{
	GPMF_stream prevstate;

	if (ms)
	{
		uint32_t curr_level = ms->nest_level;

		memcpy(&prevstate, ms, sizeof(GPMF_stream));

		if (ms->pos < ms->buffer_size_longs && curr_level > 0)
		{

			do
			{
				ms->last_seek[curr_level] = ms->pos;
				ms->pos = ms->last_level_pos[curr_level - 1] + 2;
				ms->nest_size[curr_level] += ms->last_seek[curr_level] - ms->pos;
				do
				{
					if (ms->last_seek[curr_level] > ms->pos && ms->buffer[ms->pos] == fourcc)
					{

						return GPMF_OK; //found match
					}
				} while (ms->last_seek[curr_level] > ms->pos && 0 == GPMF_Next(ms, GPMF_CURRENT_LEVEL));

				curr_level--;
			} while (recurse == GPMF_RECURSE_LEVELS && curr_level > 0);

			memcpy(ms, &prevstate, sizeof(GPMF_stream));

			return GPMF_ERROR_FIND;
		}
	}

	return GPMF_ERROR_FIND;
}
