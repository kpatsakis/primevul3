Bool gf_fs_solve_js_script(char *szPath, const char *file_name, const char *file_ext)
{
	const char *js_dirs;
	if (gf_opts_default_shared_directory(szPath)) {
		strcat(szPath, "/scripts/jsf/");
		GF_LOG(GF_LOG_DEBUG, GF_LOG_FILTER, ("Trying JS filter %s\n", szPath));
		if (locate_js_script(szPath, file_name, file_ext)) {
			return GF_TRUE;
		}
	} else {
		GF_LOG(GF_LOG_INFO, GF_LOG_FILTER, ("Failed to get default shared dir\n"));
	}
	js_dirs = gf_opts_get_key("core", "js-dirs");
	while (js_dirs && js_dirs[0]) {
		char *sep = strchr(js_dirs, ',');
		if (sep) {
			u32 cplen = (u32) (sep-js_dirs);
			if (cplen>=GF_MAX_PATH) cplen = GF_MAX_PATH-1;
			strncpy(szPath, js_dirs, cplen);
			szPath[cplen]=0;
			js_dirs = sep+1;
		} else {
			strcpy(szPath, js_dirs);
		}
		if (strcmp(szPath, "$GJS")) {
			u32 len = (u32) strlen(szPath);
			if (len && (szPath[len-1]!='/') && (szPath[len-1]!='\\'))
				strcat(szPath, "/");
			GF_LOG(GF_LOG_DEBUG, GF_LOG_FILTER, ("Trying JS filter in %s\n", szPath));
			if (locate_js_script(szPath, file_name, file_ext))
				return GF_TRUE;
		}
		if (!sep) break;
	}
	return GF_FALSE;
}