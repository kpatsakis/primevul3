struct PE_(r_bin_pe_obj_t)* PE_(r_bin_pe_new)(const char* file, bool verbose) {
	ut8* buf;
	struct PE_(r_bin_pe_obj_t)* bin = R_NEW0 (struct PE_(r_bin_pe_obj_t));
	if (!bin) {
		return NULL;
	}
	bin->file = file;
	if (!(buf = (ut8*) r_file_slurp (file, &bin->size))) {
		return PE_(r_bin_pe_free)(bin);
	}
	bin->b = r_buf_new ();
	if (!r_buf_set_bytes (bin->b, buf, bin->size)) {
		free (buf);
		return PE_(r_bin_pe_free)(bin);
	}
	bin->verbose = verbose;
	free (buf);
	if (!bin_pe_init (bin)) {
		return PE_(r_bin_pe_free)(bin);
	}
	return bin;
}
