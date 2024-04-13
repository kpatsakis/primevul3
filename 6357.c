wait_for_no_updates(MYSQL *connection, uint timeout, uint threshold)
{
	time_t	start_time;

	start_time = time(NULL);

	msg_ts("Waiting %u seconds for queries running longer than %u seconds "
	       "to finish\n", timeout, threshold);

	while (time(NULL) <= (time_t)(start_time + timeout)) {
		if (!have_queries_to_wait_for(connection, threshold)) {
			return(true);
		}
		os_thread_sleep(1000000);
	}

	msg_ts("Unable to obtain lock. Please try again later.");

	return(false);
}