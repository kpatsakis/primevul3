start_query_killer()
{
	kill_query_thread_stop    = os_event_create("kill_query_thread_stop");
	kill_query_thread_started = os_event_create("kill_query_thread_started");
	kill_query_thread_stopped = os_event_create("kill_query_thread_stopped");

	os_thread_create(kill_query_thread, NULL, &kill_query_thread_id);

	os_event_wait(kill_query_thread_started);
}