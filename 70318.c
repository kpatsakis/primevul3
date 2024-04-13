GPMF_ERR GPMF_DeviceName(GPMF_stream *ms, char *devicenamebuf, uint32_t devicename_buf_size)
{
	if (ms && devicenamebuf)
	{
		uint32_t len = (uint32_t)strlen(ms->device_name);
		if (len >= devicename_buf_size)
			return GPMF_ERROR_MEMORY;

		memcpy(devicenamebuf, ms->device_name, len);
		devicenamebuf[len] = 0;
		return GPMF_OK;
	}
	return GPMF_ERROR_MEMORY;
}
