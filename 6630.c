GF_Err gf_fs_get_last_connect_error(GF_FilterSession *fs)
{
	GF_Err e;
	if (!fs) return GF_BAD_PARAM;
	e = fs->last_connect_error;
	fs->last_connect_error = GF_OK;
	return e;
}