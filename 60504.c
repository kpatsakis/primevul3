evutil_free_globals_(void)
{
	evutil_free_secure_rng_globals_();
	evutil_free_sock_err_globals();
}
