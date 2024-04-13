rdpsnd_process(STREAM s)
{
	uint16 len;

	while (!s_check_end(s))
	{
		/* New packet */
		if (packet.size == 0)
		{
			if ((s->end - s->p) < 4)
			{
				logger(Sound, Error,
				       "rdpsnd_process(), split at packet header, things will go south from here...");
				return;
			}
			in_uint8(s, packet_opcode);
			in_uint8s(s, 1);	/* Padding */
			in_uint16_le(s, len);

			logger(Sound, Debug, "rdpsnd_process(), Opcode = 0x%x Length= %d",
			       (int) packet_opcode, (int) len);

			packet.p = packet.data;
			packet.end = packet.data + len;
			packet.size = len;
		}
		else
		{
			len = MIN(s->end - s->p, packet.end - packet.p);

			/* Microsoft's server is so broken it's not even funny... */
			if (packet_opcode == SNDC_WAVE)
			{
				if ((packet.p - packet.data) < 12)
					len = MIN(len, 12 - (packet.p - packet.data));
				else if ((packet.p - packet.data) == 12)
				{
					logger(Sound, Debug,
					       "rdpsnd_process(), eating 4 bytes of %d bytes...",
					       len);
					in_uint8s(s, 4);
					len -= 4;
				}
			}

			in_uint8a(s, packet.p, len);
			packet.p += len;
		}

		/* Packet fully assembled */
		if (packet.p == packet.end)
		{
			packet.p = packet.data;
			rdpsnd_process_packet(packet_opcode, &packet);
			packet.size = 0;
		}
	}
}
