u32 gf_fs_get_filters_count(GF_FilterSession *session)
{
	return session ? gf_list_count(session->filters) : 0;
}