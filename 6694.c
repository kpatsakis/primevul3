GF_Err gf_fs_get_filter_stats(GF_FilterSession *session, u32 idx, GF_FilterStats *stats)
{
	if (!stats || !session) return GF_BAD_PARAM;
	return gf_filter_get_stats(gf_list_get(session->filters, idx), stats);
}