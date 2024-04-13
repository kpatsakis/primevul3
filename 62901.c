static UINT drdynvc_virtual_channel_event_disconnected(drdynvcPlugin* drdynvc)
{
	UINT status;

	if (drdynvc->OpenHandle == 0)
		return CHANNEL_RC_OK;

	if (!drdynvc)
		return CHANNEL_RC_BAD_CHANNEL_HANDLE;

	if (!MessageQueue_PostQuit(drdynvc->queue, 0))
	{
		status = GetLastError();
		WLog_Print(drdynvc->log, WLOG_ERROR, "MessageQueue_PostQuit failed with error %"PRIu32"", status);
		return status;
	}

	if (WaitForSingleObject(drdynvc->thread, INFINITE) != WAIT_OBJECT_0)
	{
		status = GetLastError();
		WLog_Print(drdynvc->log, WLOG_ERROR, "WaitForSingleObject failed with error %"PRIu32"", status);
		return status;
	}

	MessageQueue_Free(drdynvc->queue);
	CloseHandle(drdynvc->thread);
	drdynvc->queue = NULL;
	drdynvc->thread = NULL;
	status = drdynvc->channelEntryPoints.pVirtualChannelCloseEx(drdynvc->InitHandle,
	         drdynvc->OpenHandle);

	if (status != CHANNEL_RC_OK)
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "pVirtualChannelClose failed with %s [%08"PRIX32"]",
		           WTSErrorToString(status), status);
	}

	drdynvc->OpenHandle = 0;

	if (drdynvc->data_in)
	{
		Stream_Free(drdynvc->data_in, TRUE);
		drdynvc->data_in = NULL;
	}

	if (drdynvc->channel_mgr)
	{
		dvcman_free(drdynvc, drdynvc->channel_mgr);
		drdynvc->channel_mgr = NULL;
	}

	return status;
}
