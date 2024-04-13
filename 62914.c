static void dvcman_free(drdynvcPlugin* drdynvc, IWTSVirtualChannelManager* pChannelMgr)
{
	int i;
	IWTSPlugin* pPlugin;
	DVCMAN_LISTENER* listener;
	DVCMAN* dvcman = (DVCMAN*) pChannelMgr;
	UINT error;
	ArrayList_Free(dvcman->channels);

	for (i = 0; i < dvcman->num_listeners; i++)
	{
		listener = (DVCMAN_LISTENER*) dvcman->listeners[i];
		free(listener->channel_name);
		free(listener);
	}

	dvcman->num_listeners = 0;

	for (i = 0; i < dvcman->num_plugins; i++)
	{
		pPlugin = dvcman->plugins[i];

		if (pPlugin->Terminated)
			if ((error = pPlugin->Terminated(pPlugin)))
				WLog_Print(drdynvc->log, WLOG_ERROR, "Terminated failed with error %"PRIu32"!", error);
	}

	dvcman->num_plugins = 0;
	StreamPool_Free(dvcman->pool);
	free(dvcman);
}
