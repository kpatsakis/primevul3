process_new_pointer_pdu(STREAM s)
{
	int xor_bpp;
	logger(Protocol, Debug, "%s()", __func__);


	in_uint16_le(s, xor_bpp);
	process_colour_pointer_common(s, xor_bpp);
}
