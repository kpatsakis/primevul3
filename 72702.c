static Bool wgt_enum_files(void *cbck, char *file_name, char *file_path, GF_FileEnumInfo *file_info)
{
	WGTEnum *wgt = (WGTEnum *)cbck;

	if (!strcmp(wgt->root_file, file_path)) return 0;
	/*remove CVS stuff*/
	if (strstr(file_path, ".#")) return 0;
	gf_list_add(wgt->imports, gf_strdup(file_path) );
	return 0;
}
