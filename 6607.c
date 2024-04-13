GF_Err gf_fs_set_max_sleep_time(GF_FilterSession *session, u32 max_sleep)
{
	if (!session) return GF_BAD_PARAM;
	session->max_sleep = max_sleep;
	return GF_OK;
}