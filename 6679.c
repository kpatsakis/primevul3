GF_DownloadManager *gf_filter_get_download_manager(GF_Filter *filter)
{
	if (!filter) return NULL;
	return gf_fs_get_download_manager(filter->session);
}