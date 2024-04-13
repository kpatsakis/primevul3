static UINT dvcman_init(drdynvcPlugin* drdynvc, IWTSVirtualChannelManager* pChannelMgr)
{
	int i;
	IWTSPlugin* pPlugin;
	DVCMAN* dvcman = (DVCMAN*) pChannelMgr;
	UINT error;

	for (i = 0; i < dvcman->num_plugins; i++)
	{
		pPlugin = dvcman->plugins[i];

		if (pPlugin->Initialize)
			if ((error = pPlugin->Initialize(pPlugin, pChannelMgr)))
			{
				WLog_Print(drdynvc->log, WLOG_ERROR, "Initialize failed with error %"PRIu32"!", error);
				return error;
			}
	}

	return CHANNEL_RC_OK;
}
