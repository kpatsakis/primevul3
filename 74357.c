static int bin_signature(RCore *r, int mode) {
	RBinFile *cur = r_bin_cur (r->bin);
	RBinPlugin *plg = r_bin_file_cur_plugin (cur);
	if (plg && plg->signature) {
		const char *signature = plg->signature (cur, IS_MODE_JSON (mode));
		r_cons_println (signature);
		free ((char*) signature);
		return true;
	}
	return false;
}
