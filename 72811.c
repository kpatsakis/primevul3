process_ts_logon_info_extended(STREAM s)
{
	uint32 fieldspresent;
	uint32 len;
	uint32 version;

	logger(Protocol, Debug, "%s()", __func__);

	in_uint8s(s, 2);	/* Length */
	in_uint32_le(s, fieldspresent);
	if (fieldspresent & LOGON_EX_AUTORECONNECTCOOKIE)
	{
		/* TS_LOGON_INFO_FIELD */
		in_uint8s(s, 4);	/* cbFieldData */

		/* ARC_SC_PRIVATE_PACKET */
		in_uint32_le(s, len);
		if (len != 28)
		{
			logger(Protocol, Error,
			       "process_ts_logon_info_extended(), invalid length in Auto-Reconnect packet");
			return;
		}

		in_uint32_le(s, version);
		if (version != 1)
		{
			logger(Protocol, Error,
			       "process_ts_logon_info_extended(), unsupported version of Auto-Reconnect packet");
			return;
		}

		in_uint32_le(s, g_reconnect_logonid);
		in_uint8a(s, g_reconnect_random, 16);
		g_has_reconnect_random = True;
		g_reconnect_random_ts = time(NULL);
		logger(Protocol, Debug,
		       "process_ts_logon_info_extended(), saving Auto-Reconnect cookie, id=%u",
		       g_reconnect_logonid);

		gettimeofday(&g_pending_resize_defer_timer, NULL);
	}
}
