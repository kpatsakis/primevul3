stop_query_killer()
{
	os_event_set(kill_query_thread_stop);
	os_event_wait_time(kill_query_thread_stopped, 60000);
}