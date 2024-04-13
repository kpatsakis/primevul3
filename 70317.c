uint32_t GPMF_DeviceID(GPMF_stream *ms)
{
	if (ms)
	{
		return ms->device_id;
	}
	return 0;
}
