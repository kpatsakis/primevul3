process_ts_set_error_info_pdu(STREAM s, uint32 * ext_disc_reason)
{
	in_uint32_le(s, *ext_disc_reason);

	logger(Protocol, Debug, "process_ts_set_error_info_pdu(), error info = %d",
	       *ext_disc_reason);
}
