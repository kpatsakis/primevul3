rdp_out_ts_multifragmentupdate_capabilityset(STREAM s)
{
	out_uint16_le(s, RDP_CAPSET_MULTIFRAGMENTUPDATE);
	out_uint16_le(s, RDP_CAPLEN_MULTIFRAGMENTUPDATE);
	out_uint32_le(s, RDESKTOP_FASTPATH_MULTIFRAGMENT_MAX_SIZE);	/* MaxRequestSize */
}
