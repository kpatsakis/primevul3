kill_query_thread(
/*===============*/
	void *arg __attribute__((unused)))
{
	MYSQL	*mysql;
	time_t	start_time;

	my_thread_init();

	start_time = time(NULL);

	os_event_set(kill_query_thread_started);

	msg_ts("Kill query timeout %d seconds.\n",
	       opt_kill_long_queries_timeout);

	while (time(NULL) - start_time <
				(time_t)opt_kill_long_queries_timeout) {
		if (os_event_wait_time(kill_query_thread_stop, 1000) !=
		    OS_SYNC_TIME_EXCEEDED) {
			goto stop_thread;
		}
	}

	if ((mysql = xb_mysql_connect()) == NULL) {
		msg("Error: kill query thread failed\n");
		goto stop_thread;
	}

	while (true) {
		kill_long_queries(mysql, time(NULL) - start_time);
		if (os_event_wait_time(kill_query_thread_stop, 1000) !=
		    OS_SYNC_TIME_EXCEEDED) {
			break;
		}
	}

	mysql_close(mysql);

stop_thread:
	msg_ts("Kill query thread stopped\n");

	my_thread_end();

	os_event_set(kill_query_thread_stopped);

	os_thread_exit();
	OS_THREAD_DUMMY_RETURN;
}