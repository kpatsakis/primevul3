evutil_global_setup_locks_(const int enable_locks)
{
	EVTHREAD_SETUP_GLOBAL_LOCK(windows_socket_errors_lock_, 0);
	return 0;
}
