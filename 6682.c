GF_Filter *gf_fs_new_filter(GF_FilterSession *fsess, const char *name, GF_Err *e)
{
	GF_Filter *f;
	char szRegName[25];
	GF_FilterRegister *reg;

	GF_SAFEALLOC(reg, GF_FilterRegister);
	if (!reg) return NULL;

	reg->flags = 0;
#ifndef GPAC_DISABLE_DOC
	reg->author = "custom";
	reg->description = "custom";
	reg->help = "custom";
#endif
	reg->version = "custom";
	sprintf(szRegName, "custom%p", reg);
	reg->name = gf_strdup(name ? name : szRegName);
	reg->flags = GF_FS_REG_CUSTOM | GF_FS_REG_EXPLICIT_ONLY;

	f = gf_filter_new(fsess, reg, NULL, NULL, 0, e, NULL, GF_FALSE);
	if (!f) return NULL;
	if (name)
		gf_filter_set_name(f, name);
	return f;
}