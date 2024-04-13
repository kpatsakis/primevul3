static void loop_unprepare_queue(struct loop_device *lo)
{
	kthread_flush_worker(&lo->worker);
	kthread_stop(lo->worker_task);
}
