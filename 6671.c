const GF_FilterRegister * gf_fs_get_filter_register(GF_FilterSession *fsess, u32 idx)
{
	return gf_list_get(fsess->registry, idx);
}