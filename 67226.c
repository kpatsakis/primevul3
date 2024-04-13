static Sdb* Pe_r_bin_store_string_file_info(StringFileInfo* stringFileInfo) {
	char key[30];
	int i = 0;
	Sdb* sdb = NULL;
	if (!stringFileInfo) {
		return NULL;
	}
	sdb = sdb_new0 ();
	if (!sdb) {
		return NULL;
	}
	for (; i < stringFileInfo->numOfChildren; i++) {
		snprintf (key, 30, "stringtable%d", i);
		sdb_ns_set (sdb, key, Pe_r_bin_store_string_table (stringFileInfo->Children[i]));
	}
	return sdb;
}
