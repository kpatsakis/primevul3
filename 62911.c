static UINT dvcman_create_channel(drdynvcPlugin* drdynvc,
                                  IWTSVirtualChannelManager* pChannelMgr,
                                  UINT32 ChannelId, const char* ChannelName)
{
	int i;
	BOOL bAccept;
	DVCMAN_LISTENER* listener;
	DVCMAN_CHANNEL* channel;
	DrdynvcClientContext* context;
	IWTSVirtualChannelCallback* pCallback;
	DVCMAN* dvcman = (DVCMAN*) pChannelMgr;
	UINT error;

	if (!(channel = dvcman_channel_new(drdynvc, pChannelMgr, ChannelId, ChannelName)))
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "dvcman_channel_new failed!");
		return CHANNEL_RC_NO_MEMORY;
	}

	channel->status = ERROR_NOT_CONNECTED;
	ArrayList_Add(dvcman->channels, channel);

	for (i = 0; i < dvcman->num_listeners; i++)
	{
		listener = (DVCMAN_LISTENER*) dvcman->listeners[i];

		if (strcmp(listener->channel_name, ChannelName) == 0)
		{
			channel->iface.Write = dvcman_write_channel;
			channel->iface.Close = dvcman_close_channel_iface;
			bAccept = TRUE;
			pCallback = NULL;

			if ((error = listener->listener_callback->OnNewChannelConnection(
			                 listener->listener_callback,
			                 (IWTSVirtualChannel*) channel, NULL, &bAccept, &pCallback)) == CHANNEL_RC_OK
			    && bAccept)
			{
				WLog_Print(drdynvc->log, WLOG_DEBUG, "listener %s created new channel %"PRIu32"",
				           listener->channel_name, channel->channel_id);
				channel->status = CHANNEL_RC_OK;
				channel->channel_callback = pCallback;
				channel->pInterface = listener->iface.pInterface;
				context = dvcman->drdynvc->context;
				IFCALLRET(context->OnChannelConnected, error, context, ChannelName,
				          listener->iface.pInterface);

				if (error)
					WLog_Print(drdynvc->log, WLOG_ERROR, "context.OnChannelConnected failed with error %"PRIu32"",
					           error);

				return error;
			}
			else
			{
				if (error)
				{
					WLog_Print(drdynvc->log, WLOG_ERROR, "OnNewChannelConnection failed with error %"PRIu32"!", error);
					return error;
				}
				else
				{
					WLog_Print(drdynvc->log, WLOG_ERROR, "OnNewChannelConnection returned with bAccept FALSE!");
					return ERROR_INTERNAL_ERROR;
				}
			}
		}
	}

	return ERROR_INTERNAL_ERROR;
}
