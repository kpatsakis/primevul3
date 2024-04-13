static inline int is_thumb(struct PE_(r_bin_pe_obj_t)* bin) {
	return bin->nt_headers->optional_header.AddressOfEntryPoint & 1;
}
