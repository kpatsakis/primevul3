Bool gf_fs_enum_unmapped_options(GF_FilterSession *fsess, u32 *idx, char **argname, u32 *argtype)
{
	if (!fsess || !fsess->parsed_args) return GF_FALSE;
	u32 i, count = gf_list_count(fsess->parsed_args);

	for (i=*idx; i<count; i++) {
		GF_FSArgItem *ai = gf_list_get(fsess->parsed_args, i);
		(*idx)++;
		if (ai->found_type) continue;
		if (argname) *argname = ai->argname;
		if (argtype) *argtype = ai->type;
		return GF_TRUE;
	}
	return GF_FALSE;
}