rdpdr_send_client_name_request(void)
{
	/* DR_CORE_CLIENT_NAME_REQ */
	STREAM s;
	struct stream name = { 0 };

	if (NULL == g_rdpdr_clientname)
	{
		g_rdpdr_clientname = g_hostname;
	}

	s_realloc(&name, 512 * 4);
	s_reset(&name);
	out_utf16s(&name, g_rdpdr_clientname);
	s_mark_end(&name);

	s = channel_init(rdpdr_channel, 16 + s_length(&name));
	out_uint16_le(s, RDPDR_CTYP_CORE);
	out_uint16_le(s, PAKID_CORE_CLIENT_NAME);
	out_uint32_le(s, 1);	/* UnicodeFlag */
	out_uint32_le(s, 0);	/* CodePage */
	out_uint32_le(s, s_length(&name));	/* ComputerNameLen */
	out_stream(s, &name);
	s_mark_end(s);
	channel_send(s, rdpdr_channel);
}
