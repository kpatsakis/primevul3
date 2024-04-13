sigterm(int sig)
{
	(void)sig;
	/*int save_errno = errno;*/
	/*signal(sig, SIG_IGN);*/
#if 0
	/* calling syslog() is forbidden in a signal handler according to
	 * signal(3) */
	syslog(LOG_NOTICE, "received signal %d, good-bye", sig);
#endif
	quitting = 1;
	/*errno = save_errno;*/
}
