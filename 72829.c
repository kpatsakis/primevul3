rdp_out_ts_font_capabilityset(STREAM s)
{
	uint16 flags = FONTSUPPORT_FONTLIST;

	out_uint16_le(s, RDP_CAPSET_FONT);
	out_uint16_le(s, RDP_CAPLEN_FONT);

	out_uint16_le(s, flags);	/* fontSupportFlags */
	out_uint16_le(s, 0);	/* pad2octets */
}
