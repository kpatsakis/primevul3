static int bin_mem(RCore *r, int mode) {
	RList *mem = NULL;
	if (!r) {
		return false;
	}
	if (!IS_MODE_JSON (mode)) {
		if (!(IS_MODE_RAD (mode) || IS_MODE_SET (mode))) {
			r_cons_println ("[Memory]\n");
		}
	}
	if (!(mem = r_bin_get_mem (r->bin))) {
		if (IS_MODE_JSON (mode)) {
			r_cons_print("[]");
		}
		return false;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("[");
		bin_mem_print (mem, 7, 0, R_MODE_JSON);
		r_cons_println ("]");
		return true;
	} else if (!(IS_MODE_RAD (mode) || IS_MODE_SET (mode))) {
		bin_mem_print (mem, 7, 0, mode);
	}
	return true;
}
