GF_Err gf_fs_set_max_resolution_chain_length(GF_FilterSession *session, u32 max_chain_length)
{
	if (!session) return GF_BAD_PARAM;
	session->max_resolve_chain_len = max_chain_length;
	return GF_OK;
}