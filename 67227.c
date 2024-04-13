static Sdb* Pe_r_bin_store_var_file_info(VarFileInfo* varFileInfo) {
	char key[20];
	unsigned int i = 0;
	if (!varFileInfo) {
		return NULL;
	}
	Sdb* sdb = sdb_new0 ();
	if (!sdb) {
		return NULL;
	}
	for (; i < varFileInfo->numOfChildren; i++) {
		snprintf (key, 20, "var%d", i);
		sdb_ns_set (sdb, key, Pe_r_bin_store_var (varFileInfo->Children[i]));
	}
	return sdb;
}
