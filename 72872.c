rdpsnd_init_packet(uint8 type, uint16 size)
{
	STREAM s;

	s = channel_init(rdpsnd_channel, size + 4);
	out_uint8(s, type);
	out_uint8(s, 0);	/* protocol-mandated padding */
	out_uint16_le(s, size);
	return s;
}
