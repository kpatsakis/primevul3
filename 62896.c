static UINT drdynvc_send_capability_response(drdynvcPlugin* drdynvc)
{
	UINT status;
	wStream* s;

	if (!drdynvc)
		return CHANNEL_RC_BAD_CHANNEL_HANDLE;

	WLog_Print(drdynvc->log, WLOG_TRACE, "capability_response");
	s = Stream_New(NULL, 4);

	if (!s)
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "Stream_Ndrdynvc_write_variable_uintew failed!");
		return CHANNEL_RC_NO_MEMORY;
	}

	Stream_Write_UINT16(s,
	                    0x0050); /* Cmd+Sp+cbChId+Pad. Note: MSTSC sends 0x005c */
	Stream_Write_UINT16(s, drdynvc->version);
	status = drdynvc_send(drdynvc, s);

	if (status != CHANNEL_RC_OK)
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "VirtualChannelWriteEx failed with %s [%08"PRIX32"]",
		           WTSErrorToString(status), status);
	}

	return status;
}
