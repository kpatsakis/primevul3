GF_Filter *gf_fs_get_filter(GF_FilterSession *session, u32 idx)
{
	return session ? gf_list_get(session->filters, idx) : NULL;
}