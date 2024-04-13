static int loop_prepare_queue(struct loop_device *lo)
{
	kthread_init_worker(&lo->worker);
	lo->worker_task = kthread_run(loop_kthread_worker_fn,
			&lo->worker, "loop%d", lo->lo_number);
	if (IS_ERR(lo->worker_task))
		return -ENOMEM;
	set_user_nice(lo->worker_task, MIN_NICE);
	return 0;
}
