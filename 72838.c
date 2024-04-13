rdp_process_general_caps(STREAM s)
{
	uint16 pad2octetsB;	/* rdp5 flags? */

	logger(Protocol, Debug, "%s()", __func__);

	in_uint8s(s, 10);
	in_uint16_le(s, pad2octetsB);

	if (!pad2octetsB)
		g_rdp_version = RDP_V4;
}
