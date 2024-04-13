static GF_DownloadManager *gf_fs_get_download_manager(GF_FilterSession *fs)
{
	if (!fs->download_manager) {
		fs->download_manager = gf_dm_new(fs);

		gf_dm_set_auth_callback(fs->download_manager, gf_fsess_get_user_pass, fs);
	}
	return fs->download_manager;
}