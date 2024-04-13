Bool gf_fs_filter_exists(GF_FilterSession *fsess, const char *name)
{
	u32 i, count;

	if (!strcmp(name, "enc")) return GF_TRUE;

	count = gf_list_count(fsess->registry);
	for (i=0;i<count;i++) {
		const GF_FilterRegister *f_reg = gf_list_get(fsess->registry, i);
		if (!strcmp(f_reg->name, name)) {
			return GF_TRUE;
		}
	}
	return GF_FALSE;
}