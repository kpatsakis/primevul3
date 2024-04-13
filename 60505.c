evutil_free_sock_err_globals(void)
{
	struct cached_sock_errs_entry **errs, *tofree;

	for (errs = HT_START(cached_sock_errs_map, &windows_socket_errors)
		     ; errs; ) {
		tofree = *errs;
		errs = HT_NEXT_RMV(cached_sock_errs_map,
				   &windows_socket_errors,
				   errs);
		LocalFree(tofree->msg);
		mm_free(tofree);
	}

	HT_CLEAR(cached_sock_errs_map, &windows_socket_errors);

#ifndef EVENT__DISABLE_THREAD_SUPPORT
	if (windows_socket_errors_lock_ != NULL) {
		EVTHREAD_FREE_LOCK(windows_socket_errors_lock_, 0);
		windows_socket_errors_lock_ = NULL;
	}
#endif
}
