sec_process_srv_info(STREAM s)
{
	in_uint16_le(s, g_server_rdp_version);
	logger(Protocol, Debug, "sec_process_srv_info(), server RDP version is %d",
	       g_server_rdp_version);
	if (1 == g_server_rdp_version)
	{
		g_rdp_version = RDP_V4;
		g_server_depth = 8;
	}
}
