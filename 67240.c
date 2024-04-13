static int bin_pe_init_overlay(struct PE_(r_bin_pe_obj_t)* bin) {
	ut64 pe_overlay_size;
	ut64 pe_overlay_offset = PE_(bin_pe_get_overlay) (bin, &pe_overlay_size);
	if (pe_overlay_offset) {
		sdb_num_set (bin->kv, "pe_overlay.offset", pe_overlay_offset, 0);
		sdb_num_set (bin->kv, "pe_overlay.size", pe_overlay_size, 0);
	}
	return 0;
}
