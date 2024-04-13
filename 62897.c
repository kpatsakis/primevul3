static DWORD WINAPI drdynvc_virtual_channel_client_thread(LPVOID arg)
{
	wStream* data;
	wMessage message;
	UINT error = CHANNEL_RC_OK;
	drdynvcPlugin* drdynvc = (drdynvcPlugin*) arg;

	if (!drdynvc)
	{
		ExitThread((DWORD) CHANNEL_RC_BAD_CHANNEL_HANDLE);
		return CHANNEL_RC_BAD_CHANNEL_HANDLE;
	}

	while (1)
	{
		if (!MessageQueue_Wait(drdynvc->queue))
		{
			WLog_Print(drdynvc->log, WLOG_ERROR, "MessageQueue_Wait failed!");
			error = ERROR_INTERNAL_ERROR;
			break;
		}

		if (!MessageQueue_Peek(drdynvc->queue, &message, TRUE))
		{
			WLog_Print(drdynvc->log, WLOG_ERROR, "MessageQueue_Peek failed!");
			error = ERROR_INTERNAL_ERROR;
			break;
		}

		if (message.id == WMQ_QUIT)
			break;

		if (message.id == 0)
		{
			data = (wStream*) message.wParam;

			if ((error = drdynvc_order_recv(drdynvc, data)))
			{
				Stream_Free(data, TRUE);
				WLog_Print(drdynvc->log, WLOG_ERROR, "drdynvc_order_recv failed with error %"PRIu32"!", error);
				break;
			}

			Stream_Free(data, TRUE);
		}
	}

	{
		/* Disconnect remaining dynamic channels that the server did not.
		* This is required to properly shut down channels by calling the appropriate
		* event handlers. */
		DVCMAN* drdynvcMgr = (DVCMAN*)drdynvc->channel_mgr;

		while (ArrayList_Count(drdynvcMgr->channels) > 0)
		{
			IWTSVirtualChannel* channel = (IWTSVirtualChannel*)
			                              ArrayList_GetItem(drdynvcMgr->channels, 0);
			const UINT32 ChannelId = drdynvc->channel_mgr->GetChannelId(channel);
			dvcman_close_channel(drdynvc->channel_mgr, ChannelId);
		}
	}

	if (error && drdynvc->rdpcontext)
		setChannelError(drdynvc->rdpcontext, error,
		                "drdynvc_virtual_channel_client_thread reported an error");

	ExitThread((DWORD) error);
	return error;
}
