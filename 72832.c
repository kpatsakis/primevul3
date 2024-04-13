rdp_out_ts_large_pointer_capabilityset(STREAM s)
{
	uint16 flags = LARGE_POINTER_FLAG_96x96;

	out_uint16_le(s, RDP_CAPSET_LARGE_POINTER);
	out_uint16_le(s, RDP_CAPLEN_LARGE_POINTER);
	out_uint16_le(s, flags);	/* largePointerSupportFlags */
}
