static bool get_rsds(ut8* dbg_data, int dbg_data_len, SCV_RSDS_HEADER* res) {
	const int rsds_sz = 4 + sizeof (SGUID) + 4;
	if (dbg_data_len < rsds_sz) {
		return false;
	}
	memcpy (res, dbg_data, rsds_sz);
	res->file_name = (ut8*) strdup ((const char*) dbg_data + rsds_sz);
	return true;
}
