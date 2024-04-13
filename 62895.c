static UINT drdynvc_send(drdynvcPlugin* drdynvc, wStream* s)
{
	UINT status;

	if (!drdynvc)
		status = CHANNEL_RC_BAD_CHANNEL_HANDLE;
	else
	{
		status = drdynvc->channelEntryPoints.pVirtualChannelWriteEx(drdynvc->InitHandle,
		         drdynvc->OpenHandle, Stream_Buffer(s), (UINT32) Stream_GetPosition(s), s);
	}

	switch (status)
	{
		case CHANNEL_RC_OK:
			return CHANNEL_RC_OK;

		case CHANNEL_RC_NOT_CONNECTED:
			Stream_Free(s, TRUE);
			return CHANNEL_RC_OK;

		case CHANNEL_RC_BAD_CHANNEL_HANDLE:
			Stream_Free(s, TRUE);
			WLog_ERR(TAG, "VirtualChannelWriteEx failed with CHANNEL_RC_BAD_CHANNEL_HANDLE");
			return status;

		default:
			Stream_Free(s, TRUE);
			WLog_Print(drdynvc->log, WLOG_ERROR, "VirtualChannelWriteEx failed with %s [%08"PRIX32"]",
			           WTSErrorToString(status),
			           status);
			return status;
	}
}
