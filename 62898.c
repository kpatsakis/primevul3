static UINT drdynvc_virtual_channel_event_attached(drdynvcPlugin* drdynvc)
{
	int i;
	DVCMAN* dvcman;

	if (!drdynvc)
		return CHANNEL_RC_BAD_CHANNEL_HANDLE;

	dvcman = (DVCMAN*) drdynvc->channel_mgr;

	if (!dvcman)
		return CHANNEL_RC_BAD_CHANNEL_HANDLE;

	for (i = 0; i < dvcman->num_plugins; i++)
	{
		UINT error;
		IWTSPlugin* pPlugin = dvcman->plugins[i];

		if (pPlugin->Attached)
			if ((error = pPlugin->Attached(pPlugin)))
			{
				WLog_Print(drdynvc->log, WLOG_ERROR, "Attach failed with error %"PRIu32"!", error);
				return error;
			}
	}

	return CHANNEL_RC_OK;
}
