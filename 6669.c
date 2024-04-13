GF_Err gf_fs_set_filter_creation_callback(GF_FilterSession *session, gf_fs_on_filter_creation on_create_destroy, void *udta)
{
	if (!session) return GF_BAD_PARAM;
	session->rt_udta = udta;
	session->on_filter_create_destroy = on_create_destroy;
	return GF_OK;
}