static int bin_libs(RCore *r, int mode) {
	RList *libs;
	RListIter *iter;
	char* lib;
	int i = 0;

	if (!(libs = r_bin_get_libs (r->bin))) {
		return false;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("[");
	} else if (IS_MODE_NORMAL (mode)) {
		r_cons_println ("[Linked libraries]");
	}
	r_list_foreach (libs, iter, lib) {
		if (IS_MODE_SET (mode)) {
		} else if (IS_MODE_RAD (mode)) {
			r_cons_printf ("\"CCa entry0 %s\"\n", lib);
		} else if (IS_MODE_JSON (mode)) {
			r_cons_printf ("%s\"%s\"", iter->p ? "," : "", lib);
		} else {
			r_cons_println (lib);
		}
		i++;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("]");
	} else if (IS_MODE_NORMAL (mode)) {
		if (i == 1) {
			r_cons_printf ("\n%i library\n", i);
		} else {
			r_cons_printf ("\n%i libraries\n", i);
		}
	}
	return true;
}
