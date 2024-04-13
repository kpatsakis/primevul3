void gf_fs_remove_filter_register(GF_FilterSession *session, GF_FilterRegister *freg)
{
	gf_list_del_item(session->registry, freg);
	gf_filter_sess_reset_graph(session, freg);
}