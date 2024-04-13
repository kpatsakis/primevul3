static bool bin_strings(RCore *r, int mode, int va) {
	RList *list;
	RBinFile *binfile = r_bin_cur (r->bin);
	RBinPlugin *plugin = r_bin_file_cur_plugin (binfile);
	int rawstr = r_config_get_i (r->config, "bin.rawstr");
	if (!binfile) {
		return false;
	}
	if (!r_config_get_i (r->config, "bin.strings")) {
		return false;
	}
	if (!plugin) {
		return false;
	}
	if (plugin->info && plugin->name) {
		if (strcmp (plugin->name, "any") == 0 && !rawstr) {
			if (IS_MODE_JSON (mode)) {
				r_cons_print("[]");
			}
			return false;
		}
	}

	if (!(list = r_bin_get_strings (r->bin))) {
		return false;
	}
	_print_strings (r, list, mode, va);
	return true;
}
