GPMF_ERR GPMF_ResetState(GPMF_stream *ms)
{
	if (ms)
	{
		ms->pos = 0;
		ms->nest_level = 0;
		ms->device_count = 0;
		ms->nest_size[ms->nest_level] = 0;
		ms->last_level_pos[ms->nest_level] = 0;
		ms->last_seek[ms->nest_level] = 0;
		ms->device_id = 0;
		ms->device_name[0] = 0;

		return GPMF_OK;
	}
	
	return GPMF_ERROR_MEMORY;
}
