void blk_start_plug(struct blk_plug *plug)
{
	struct task_struct *tsk = current;

	/*
	 * If this is a nested plug, don't actually assign it.
	 */
	if (tsk->plug)
		return;

	INIT_LIST_HEAD(&plug->list);
	INIT_LIST_HEAD(&plug->mq_list);
	INIT_LIST_HEAD(&plug->cb_list);
	/*
	 * Store ordering should not be needed here, since a potential
	 * preempt will imply a full memory barrier
	 */
	tsk->plug = plug;
}
