struct _gf_ft_mgr *gf_fs_get_font_manager(GF_FilterSession *fsess)
{
#ifdef GPAC_DISABLE_PLAYER
	return NULL;
#else
	if (!fsess->font_manager) {
		fsess->font_manager = gf_font_manager_new();
	}
	return fsess->font_manager;
#endif
}