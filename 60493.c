eq_cached_sock_errs(const struct cached_sock_errs_entry *a,
		    const struct cached_sock_errs_entry *b)
{
	return a->code == b->code;
}
