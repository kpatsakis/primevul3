mcs_recv(uint16 * channel, RD_BOOL * is_fastpath, uint8 * fastpath_hdr)
{
	uint8 opcode, appid, length;
	STREAM s;

	s = iso_recv(is_fastpath, fastpath_hdr);
	if (s == NULL)
		return NULL;

	if (*is_fastpath == True)
		return s;

	in_uint8(s, opcode);
	appid = opcode >> 2;
	if (appid != MCS_SDIN)
	{
		if (appid != MCS_DPUM)
		{
			logger(Protocol, Error, "mcs_recv(), expected data, got %d", opcode);
		}
		return NULL;
	}
	in_uint8s(s, 2);	/* userid */
	in_uint16_be(s, *channel);
	in_uint8s(s, 1);	/* flags */
	in_uint8(s, length);
	if (length & 0x80)
		in_uint8s(s, 1);	/* second byte of length */
	return s;
}
