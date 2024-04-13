struct PE_(r_bin_pe_obj_t)* PE_(r_bin_pe_new_buf)(RBuffer * buf, bool verbose) {
	struct PE_(r_bin_pe_obj_t)* bin = R_NEW0 (struct PE_(r_bin_pe_obj_t));
	if (!bin) {
		return NULL;
	}
	bin->kv = sdb_new0 ();
	bin->b = r_buf_new ();
	bin->verbose = verbose;
	bin->size = buf->length;
	if (!r_buf_set_bytes (bin->b, buf->buf, bin->size)) {
		return PE_(r_bin_pe_free)(bin);
	}
	if (!bin_pe_init (bin)) {
		return PE_(r_bin_pe_free)(bin);
	}
	return bin;
}
