find_check_match(struct xt_entry_match *m, struct xt_mtchk_param *par)
{
	struct xt_match *match;
	int ret;

	match = xt_request_find_match(NFPROTO_IPV6, m->u.user.name,
				      m->u.user.revision);
	if (IS_ERR(match))
		return PTR_ERR(match);

	m->u.kernel.match = match;

	ret = check_match(m, par);
	if (ret)
		goto err;

	return 0;
err:
	module_put(m->u.kernel.match->me);
	return ret;
}
