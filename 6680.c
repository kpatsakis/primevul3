void gf_fs_print_unused_args(GF_FilterSession *fsess, const char *ignore_args)
{
	u32 idx = 0;
	char *argname;
	u32 argtype;

	while (1) {
		Bool found = GF_FALSE;
		const char *loc_arg;
		if (gf_fs_enum_unmapped_options(fsess, &idx, &argname, &argtype)==GF_FALSE)
			break;

		loc_arg = ignore_args;
		while (loc_arg) {
			u32 len;
			char *sep;
			char *match = strstr(loc_arg, argname);
			if (!match) break;
			len = (u32) strlen(argname);
			if (!match[len] || (match[len]==',')) {
				found = GF_TRUE;
				break;
			}
			sep = strchr(loc_arg, ',');
			if (!sep) break;
			loc_arg = sep+1;
		}
		if (found) continue;

		GF_LOG(GF_LOG_ERROR, GF_LOG_APP, ("Arg %s set but not used\n", argname));
	}
}