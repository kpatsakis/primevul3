static int bin_size(RCore *r, int mode) {
	ut64 size = r_bin_get_size (r->bin);
	if (IS_MODE_SIMPLE (mode) || IS_MODE_JSON (mode)) {
		r_cons_printf ("%"PFMT64u"\n", size);
	} else if (IS_MODE_RAD (mode)) {
		r_cons_printf ("f bin_size @ %"PFMT64u"\n", size);
	} else if (IS_MODE_SET (mode)) {
		r_core_cmdf (r, "f bin_size @ %"PFMT64u"\n", size);
	} else {
		r_cons_printf ("%"PFMT64u"\n", size);
	}
	return true;
}
