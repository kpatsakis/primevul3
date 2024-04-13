static void loop_queue_work(struct kthread_work *work)
{
	struct loop_cmd *cmd =
		container_of(work, struct loop_cmd, work);

	loop_handle_cmd(cmd);
}
