rdp_send_confirm_active(void)
{
	STREAM s;
	uint32 sec_flags = g_encryption ? (RDP5_FLAG | SEC_ENCRYPT) : RDP5_FLAG;
	uint16 caplen =
		RDP_CAPLEN_GENERAL +
		RDP_CAPLEN_BITMAP +
		RDP_CAPLEN_ORDER +
		RDP_CAPLEN_COLCACHE +
		RDP_CAPLEN_ACTIVATE +
		RDP_CAPLEN_CONTROL +
		RDP_CAPLEN_SHARE +
		RDP_CAPLEN_BRUSHCACHE +
		RDP_CAPLEN_INPUT +
		RDP_CAPLEN_FONT +
		RDP_CAPLEN_SOUND +
		RDP_CAPLEN_GLYPHCACHE +
		RDP_CAPLEN_MULTIFRAGMENTUPDATE +
		RDP_CAPLEN_LARGE_POINTER + 4 /* w2k fix, sessionid */ ;

	logger(Protocol, Debug, "%s()", __func__);

	if (g_rdp_version >= RDP_V5)
	{
		caplen += RDP_CAPLEN_BMPCACHE2;
		caplen += RDP_CAPLEN_NEWPOINTER;
	}
	else
	{
		caplen += RDP_CAPLEN_BMPCACHE;
		caplen += RDP_CAPLEN_POINTER;
	}

	s = sec_init(sec_flags, 6 + 14 + caplen + sizeof(RDP_SOURCE));

	out_uint16_le(s, 2 + 14 + caplen + sizeof(RDP_SOURCE));
	out_uint16_le(s, (RDP_PDU_CONFIRM_ACTIVE | 0x10));	/* Version 1 */
	out_uint16_le(s, (g_mcs_userid + 1001));

	out_uint32_le(s, g_rdp_shareid);
	out_uint16_le(s, 0x3ea);	/* userid */
	out_uint16_le(s, sizeof(RDP_SOURCE));
	out_uint16_le(s, caplen);

	out_uint8p(s, RDP_SOURCE, sizeof(RDP_SOURCE));
	out_uint16_le(s, 16);	/* num_caps */
	out_uint8s(s, 2);	/* pad */

	rdp_out_ts_general_capabilityset(s);
	rdp_out_ts_bitmap_capabilityset(s);
	rdp_out_ts_order_capabilityset(s);
	if (g_rdp_version >= RDP_V5)
	{
		rdp_out_bmpcache2_caps(s);
		rdp_out_newpointer_caps(s);
	}
	else
	{
		rdp_out_bmpcache_caps(s);
		rdp_out_pointer_caps(s);
	}
	rdp_out_colcache_caps(s);
	rdp_out_activate_caps(s);
	rdp_out_control_caps(s);
	rdp_out_share_caps(s);
	rdp_out_brushcache_caps(s);

	rdp_out_ts_input_capabilityset(s);
	rdp_out_ts_sound_capabilityset(s);
	rdp_out_ts_font_capabilityset(s);
	rdp_out_ts_glyphcache_capabilityset(s);
	rdp_out_ts_multifragmentupdate_capabilityset(s);
	rdp_out_ts_large_pointer_capabilityset(s);

	s_mark_end(s);
	sec_send(s, sec_flags);
}
