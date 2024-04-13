static int loop_kthread_worker_fn(void *worker_ptr)
{
	current->flags |= PF_LESS_THROTTLE;
	return kthread_worker_fn(worker_ptr);
}
