static UINT dvcman_create_listener(IWTSVirtualChannelManager* pChannelMgr,
                                   const char* pszChannelName, ULONG ulFlags,
                                   IWTSListenerCallback* pListenerCallback, IWTSListener** ppListener)
{
	DVCMAN* dvcman = (DVCMAN*) pChannelMgr;
	DVCMAN_LISTENER* listener;

	if (dvcman->num_listeners < MAX_PLUGINS)
	{
		WLog_DBG(TAG, "create_listener: %d.%s.", dvcman->num_listeners, pszChannelName);
		listener = (DVCMAN_LISTENER*) calloc(1, sizeof(DVCMAN_LISTENER));

		if (!listener)
		{
			WLog_ERR(TAG, "calloc failed!");
			return CHANNEL_RC_NO_MEMORY;
		}

		listener->iface.GetConfiguration = dvcman_get_configuration;
		listener->iface.pInterface = NULL;
		listener->dvcman = dvcman;
		listener->channel_name = _strdup(pszChannelName);

		if (!listener->channel_name)
		{
			WLog_ERR(TAG, "_strdup failed!");
			free(listener);
			return CHANNEL_RC_NO_MEMORY;
		}

		listener->flags = ulFlags;
		listener->listener_callback = pListenerCallback;

		if (ppListener)
			*ppListener = (IWTSListener*) listener;

		dvcman->listeners[dvcman->num_listeners++] = (IWTSListener*) listener;
		return CHANNEL_RC_OK;
	}
	else
	{
		WLog_ERR(TAG, "create_listener: Maximum DVC listener number reached.");
		return ERROR_INTERNAL_ERROR;
	}
}
