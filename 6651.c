u32 gf_fs_get_http_max_rate(GF_FilterSession *fs)
{
	if (!fs->download_manager) {
		gf_fs_get_download_manager(fs);
		if (!fs->download_manager) return 0;
	}
	return gf_dm_get_data_rate(fs->download_manager);
}