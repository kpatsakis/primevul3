void gf_fs_lock_filters(GF_FilterSession *session, Bool do_lock)
{
	if (!session || !session->filters_mx) return;
	if (do_lock) gf_mx_p(session->filters_mx);
	else gf_mx_v(session->filters_mx);
}