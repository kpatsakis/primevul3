int PE_(bin_pe_get_claimed_checksum)(struct PE_(r_bin_pe_obj_t)* bin) {
	if (!bin || !bin->optional_header) {
		return 0;
	}
	return bin->optional_header->CheckSum;
}
