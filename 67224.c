static Sdb* Pe_r_bin_store_resource_version_info(PE_VS_VERSIONINFO* vs_VersionInfo) {
	Sdb* sdb = NULL;
	if (!vs_VersionInfo) {
		return NULL;
	}
	sdb = sdb_new0 ();
	if (!sdb) {
		return NULL;
	}
	if (vs_VersionInfo->Value) {
		sdb_ns_set (sdb, "fixed_file_info", Pe_r_bin_store_fixed_file_info (vs_VersionInfo->Value));
	}
	if (vs_VersionInfo->varFileInfo) {
		sdb_ns_set (sdb, "var_file_info", Pe_r_bin_store_var_file_info (vs_VersionInfo->varFileInfo));
	}
	if (vs_VersionInfo->stringFileInfo) {
		sdb_ns_set (sdb, "string_file_info", Pe_r_bin_store_string_file_info (vs_VersionInfo->stringFileInfo));
	}
	return sdb;
}
