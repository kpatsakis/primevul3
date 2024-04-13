static void free_VS_VERSIONINFO(PE_VS_VERSIONINFO* vs_VersionInfo) {
	if (vs_VersionInfo) {
		free (vs_VersionInfo->szKey);
		free (vs_VersionInfo->Value);
		free_VarFileInfo (vs_VersionInfo->varFileInfo);
		free_StringFileInfo (vs_VersionInfo->stringFileInfo);
		free (vs_VersionInfo);
	}
}
