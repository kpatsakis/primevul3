rdp_send_suppress_output_pdu(enum RDP_SUPPRESS_STATUS allowupdates)
{
	STREAM s;
	static enum RDP_SUPPRESS_STATUS current_status = ALLOW_DISPLAY_UPDATES;

	logger(Protocol, Debug, "%s()", __func__);

	if (current_status == allowupdates)
		return;

	s = rdp_init_data(12);

	out_uint8(s, allowupdates);	/* allowDisplayUpdates */
	out_uint8s(s, 3);	/* pad3Octets */

	switch (allowupdates)
	{
		case SUPPRESS_DISPLAY_UPDATES:	/* shut the server up */
			break;

		case ALLOW_DISPLAY_UPDATES:	/* receive data again */
			out_uint16_le(s, 0);	/* left */
			out_uint16_le(s, 0);	/* top */
			out_uint16_le(s, g_session_width);	/* right */
			out_uint16_le(s, g_session_height);	/* bottom */
			break;
	}

	s_mark_end(s);
	rdp_send_data(s, RDP_DATA_PDU_CLIENT_WINDOW_STATUS);
	current_status = allowupdates;
}
