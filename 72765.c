mcs_recv_cjcf(void)
{
	RD_BOOL is_fastpath;
	uint8 fastpath_hdr;
	uint8 opcode, result;
	STREAM s;

	logger(Protocol, Debug, "%s()", __func__);
	s = iso_recv(&is_fastpath, &fastpath_hdr);

	if (s == NULL)
		return False;

	in_uint8(s, opcode);
	if ((opcode >> 2) != MCS_CJCF)
	{
		logger(Protocol, Error, "mcs_recv_cjcf(), expected opcode CJcf, got %d", opcode);
		return False;
	}

	in_uint8(s, result);
	if (result != 0)
	{
		logger(Protocol, Error, "mcs_recv_cjcf(), expected result 0, got %d", result);
		return False;
	}

	in_uint8s(s, 4);	/* mcs_userid, req_chanid */
	if (opcode & 2)
		in_uint8s(s, 2);	/* join_chanid */

	return s_check_end(s);
}
