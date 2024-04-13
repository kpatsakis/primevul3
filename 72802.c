process_cached_pointer_pdu(STREAM s)
{
	uint16 cache_idx;
	logger(Protocol, Debug, "%s()", __func__);


	in_uint16_le(s, cache_idx);
	ui_set_cursor(cache_get_cursor(cache_idx));
}
