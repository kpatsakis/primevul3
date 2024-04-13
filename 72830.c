rdp_out_ts_general_capabilityset(STREAM s)
{
	uint16 extraFlags = 0;
	if (g_rdp_version >= RDP_V5)
	{
		extraFlags |= NO_BITMAP_COMPRESSION_HDR;
		extraFlags |= AUTORECONNECT_SUPPORTED;
		extraFlags |= LONG_CREDENTIALS_SUPPORTED;
		extraFlags |= FASTPATH_OUTPUT_SUPPORTED;
	}

	out_uint16_le(s, RDP_CAPSET_GENERAL);
	out_uint16_le(s, RDP_CAPLEN_GENERAL);
	out_uint16_le(s, OSMAJORTYPE_WINDOWS);	/* osMajorType */
	out_uint16_le(s, OSMINORTYPE_WINDOWSNT);	/* osMinorType */
	out_uint16_le(s, TS_CAPS_PROTOCOLVERSION);	/* protocolVersion (must be TS_CAPS_PROTOCOLVERSION) */
	out_uint16_le(s, 0);	/* pad2OctetsA */
	out_uint16_le(s, 0);	/* generalCompressionTypes (must be 0) */
	out_uint16_le(s, extraFlags);	/* extraFlags */
	out_uint16_le(s, 0);	/* updateCapabilityFlag (must be 0) */
	out_uint16_le(s, 0);	/* remoteUnshareFlag (must be 0) */
	out_uint16_le(s, 0);	/* generalCompressionLevel (must be 0) */
	out_uint8(s, 0);	/* refreshRectSupport */
	out_uint8(s, 0);	/* suppressOutputSupport */
}
