static VOID VCAPITYPE drdynvc_virtual_channel_init_event_ex(LPVOID lpUserParam, LPVOID pInitHandle,
        UINT event, LPVOID pData, UINT dataLength)
{
	UINT error = CHANNEL_RC_OK;
	drdynvcPlugin* drdynvc = (drdynvcPlugin*) lpUserParam;

	if (!drdynvc || (drdynvc->InitHandle != pInitHandle))
	{
		WLog_ERR(TAG, "drdynvc_virtual_channel_init_event: error no match");
		return;
	}

	switch (event)
	{
		case CHANNEL_EVENT_CONNECTED:
			if ((error = drdynvc_virtual_channel_event_connected(drdynvc, pData, dataLength)))
				WLog_Print(drdynvc->log, WLOG_ERROR,
				           "drdynvc_virtual_channel_event_connected failed with error %"PRIu32"", error);

			break;

		case CHANNEL_EVENT_DISCONNECTED:
			if ((error =  drdynvc_virtual_channel_event_disconnected(drdynvc)))
				WLog_Print(drdynvc->log, WLOG_ERROR,
				           "drdynvc_virtual_channel_event_disconnected failed with error %"PRIu32"", error);

			break;

		case CHANNEL_EVENT_TERMINATED:
			if ((error =  drdynvc_virtual_channel_event_terminated(drdynvc)))
				WLog_Print(drdynvc->log, WLOG_ERROR,
				           "drdynvc_virtual_channel_event_terminated failed with error %"PRIu32"", error);

			break;

		case CHANNEL_EVENT_ATTACHED:
			if ((error =  drdynvc_virtual_channel_event_attached(drdynvc)))
				WLog_Print(drdynvc->log, WLOG_ERROR,
				           "drdynvc_virtual_channel_event_attached failed with error %"PRIu32"", error);

			break;

		case CHANNEL_EVENT_DETACHED:
			if ((error =  drdynvc_virtual_channel_event_detached(drdynvc)))
				WLog_Print(drdynvc->log, WLOG_ERROR,
				           "drdynvc_virtual_channel_event_detached failed with error %"PRIu32"", error);

			break;

		default:
			break;
	}

	if (error && drdynvc->rdpcontext)
		setChannelError(drdynvc->rdpcontext, error,
		                "drdynvc_virtual_channel_init_event_ex reported an error");
}
