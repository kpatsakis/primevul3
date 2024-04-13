static Bool wgt_enum_dir(void *cbck, char *file_name, char *file_path, GF_FileEnumInfo *file_info)
{
	if (!stricmp(file_name, "cvs") || !stricmp(file_name, ".svn") || !stricmp(file_name, ".git")) return 0;
	gf_enum_directory(file_path, 0, wgt_enum_files, cbck, NULL);
	return gf_enum_directory(file_path, 1, wgt_enum_dir, cbck, NULL);
}
