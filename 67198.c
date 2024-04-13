struct MACH0_(obj_t)* MACH0_(new_buf_steal)(RBuffer *buf, bool verbose) {
	struct MACH0_(obj_t) *bin = R_NEW0 (struct MACH0_(obj_t));
	if (!bin) {
		return NULL;
	}
	bin->kv = sdb_new (NULL, "bin.mach0", 0);
	bin->size = r_buf_size (buf);
	bin->verbose = verbose;
	bin->b = buf;
	if (!init (bin)) {
		return MACH0_(mach0_free)(bin);
	}
	return bin;
}
