GF_Err gf_fs_set_http_max_rate(GF_FilterSession *fs, u32 rate)
{
	if (!fs) return GF_OK;
	if (!fs->download_manager) {
		gf_fs_get_download_manager(fs);
		if (!fs->download_manager) return GF_OUT_OF_MEM;
	}
	gf_dm_set_data_rate(fs->download_manager, rate);
	return GF_OK;
}