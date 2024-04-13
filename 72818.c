rdp_loop(RD_BOOL * deactivated, uint32 * ext_disc_reason)
{
	uint8 type;
	RD_BOOL cont = True;
	STREAM s;

	while (g_exit_mainloop == False && cont)
	{
		s = rdp_recv(&type);
		if (s == NULL)
			return False;
		switch (type)
		{
			case RDP_PDU_DEMAND_ACTIVE:
				process_demand_active(s);
				*deactivated = False;
				break;
			case RDP_PDU_DEACTIVATE:
				logger(Protocol, Debug,
				       "rdp_loop(), RDP_PDU_DEACTIVATE packet received");
				*deactivated = True;
				g_wait_for_deactivate_ts = 0;
				break;
			case RDP_PDU_REDIRECT:
			case RDP_PDU_ENHANCED_REDIRECT:
				if (process_redirect_pdu(s, !(type == RDP_PDU_REDIRECT)) == True)
				{
					g_exit_mainloop = True;
					continue;
				}
				break;
			case RDP_PDU_DATA:
				/* If we got a data PDU, we don't need to keep the password in memory
				   anymore and therefor we should clear it for security reasons. */
				if (g_password[0] != '\0')
					memset(g_password, 0, sizeof(g_password));

				process_data_pdu(s, ext_disc_reason);
				break;
			default:
				logger(Protocol, Warning,
				       "rdp_loop(), unhandled PDU type %d received", type);
		}
		cont = g_next_packet < s->end;
	}
	return True;
}
