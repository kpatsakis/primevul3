static Sdb* Pe_r_bin_store_fixed_file_info(PE_VS_FIXEDFILEINFO* vs_fixedFileInfo) {
	Sdb* sdb = NULL;
	if (!vs_fixedFileInfo) {
		return NULL;
	}
	sdb = sdb_new0 ();
	if (!sdb) {
		return NULL;
	}
	sdb_num_set (sdb, "Signature",        vs_fixedFileInfo->dwSignature,        0);
	sdb_num_set (sdb, "StrucVersion",     vs_fixedFileInfo->dwStrucVersion,     0);
	sdb_num_set (sdb, "FileVersionMS",    vs_fixedFileInfo->dwFileVersionMS,    0);
	sdb_num_set (sdb, "FileVersionLS",    vs_fixedFileInfo->dwFileVersionLS,    0);
	sdb_num_set (sdb, "ProductVersionMS", vs_fixedFileInfo->dwProductVersionMS, 0);
	sdb_num_set (sdb, "ProductVersionLS", vs_fixedFileInfo->dwProductVersionLS, 0);
	sdb_num_set (sdb, "FileFlagsMask",    vs_fixedFileInfo->dwFileFlagsMask,    0);
	sdb_num_set (sdb, "FileFlags",        vs_fixedFileInfo->dwFileFlags,        0);
	sdb_num_set (sdb, "FileOS",           vs_fixedFileInfo->dwFileOS,           0);
	sdb_num_set (sdb, "FileType",         vs_fixedFileInfo->dwFileType,         0);
	sdb_num_set (sdb, "FileSubtype",      vs_fixedFileInfo->dwFileSubtype,      0);
	sdb_num_set (sdb, "FileDateMS",       vs_fixedFileInfo->dwFileDateMS,       0);
	sdb_num_set (sdb, "FileDateLS",       vs_fixedFileInfo->dwFileDateLS,       0);
	return sdb;
}
