struct MACH0_(obj_t)* MACH0_(mach0_new)(const char* file, bool verbose) {
	ut8 *buf;
	struct MACH0_(obj_t) *bin;
	if (!(bin = malloc (sizeof (struct MACH0_(obj_t))))) {
		return NULL;
	}
	memset (bin, 0, sizeof (struct MACH0_(obj_t)));
	bin->verbose = verbose;
	bin->file = file;
	if (!(buf = (ut8*)r_file_slurp (file, &bin->size))) {
		return MACH0_(mach0_free)(bin);
	}
	bin->b = r_buf_new ();
	if (!r_buf_set_bytes (bin->b, buf, bin->size)) {
		free (buf);
		return MACH0_(mach0_free)(bin);
	}
	free (buf);
	bin->dyld_info = NULL;
	if (!init (bin)) {
		return MACH0_(mach0_free)(bin);
	}
	bin->imports_by_ord_size = 0;
	bin->imports_by_ord = NULL;
	return bin;
}
