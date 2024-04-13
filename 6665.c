u32 gf_fs_get_max_resolution_chain_length(GF_FilterSession *session)
{
	if (!session) return 0;
	return session->max_resolve_chain_len;
}