u32 gf_fs_filters_registers_count(GF_FilterSession *fsess)
{
	return fsess ? gf_list_count(fsess->registry) : 0;
}