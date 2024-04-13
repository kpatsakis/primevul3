static void VCAPITYPE drdynvc_virtual_channel_open_event_ex(LPVOID lpUserParam, DWORD openHandle,
        UINT event, LPVOID pData, UINT32 dataLength, UINT32 totalLength, UINT32 dataFlags)
{
	UINT error = CHANNEL_RC_OK;
	drdynvcPlugin* drdynvc = (drdynvcPlugin*) lpUserParam;

	if (!drdynvc || (drdynvc->OpenHandle != openHandle))
	{
		WLog_ERR(TAG, "drdynvc_virtual_channel_open_event: error no match");
		return;
	}

	switch (event)
	{
		case CHANNEL_EVENT_DATA_RECEIVED:
			if ((error = drdynvc_virtual_channel_event_data_received(drdynvc, pData, dataLength, totalLength,
			             dataFlags)))
				WLog_Print(drdynvc->log, WLOG_ERROR,
				           "drdynvc_virtual_channel_event_data_received failed with error %"PRIu32"", error);

			break;

		case CHANNEL_EVENT_WRITE_COMPLETE:
			break;

		case CHANNEL_EVENT_USER:
			break;
	}

	if (error && drdynvc->rdpcontext)
		setChannelError(drdynvc->rdpcontext, error, "drdynvc_virtual_channel_open_event reported an error");
}
