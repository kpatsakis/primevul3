void blk_cleanup_queue(struct request_queue *q)
{
	spinlock_t *lock = q->queue_lock;

	/* mark @q DYING, no new request or merges will be allowed afterwards */
	mutex_lock(&q->sysfs_lock);
	blk_set_queue_dying(q);
	spin_lock_irq(lock);

	/*
	 * A dying queue is permanently in bypass mode till released.  Note
	 * that, unlike blk_queue_bypass_start(), we aren't performing
	 * synchronize_rcu() after entering bypass mode to avoid the delay
	 * as some drivers create and destroy a lot of queues while
	 * probing.  This is still safe because blk_release_queue() will be
	 * called only after the queue refcnt drops to zero and nothing,
	 * RCU or not, would be traversing the queue by then.
	 */
	q->bypass_depth++;
	queue_flag_set(QUEUE_FLAG_BYPASS, q);

	queue_flag_set(QUEUE_FLAG_NOMERGES, q);
	queue_flag_set(QUEUE_FLAG_NOXMERGES, q);
	queue_flag_set(QUEUE_FLAG_DYING, q);
	spin_unlock_irq(lock);
	mutex_unlock(&q->sysfs_lock);

	/*
	 * Drain all requests queued before DYING marking. Set DEAD flag to
	 * prevent that q->request_fn() gets invoked after draining finished.
	 */
	blk_freeze_queue(q);
	spin_lock_irq(lock);
	queue_flag_set(QUEUE_FLAG_DEAD, q);
	spin_unlock_irq(lock);

	/*
	 * make sure all in-progress dispatch are completed because
	 * blk_freeze_queue() can only complete all requests, and
	 * dispatch may still be in-progress since we dispatch requests
	 * from more than one contexts.
	 *
	 * No need to quiesce queue if it isn't initialized yet since
	 * blk_freeze_queue() should be enough for cases of passthrough
	 * request.
	 */
	if (q->mq_ops && blk_queue_init_done(q))
		blk_mq_quiesce_queue(q);

	/* for synchronous bio-based driver finish in-flight integrity i/o */
	blk_flush_integrity();

	/* @q won't process any more request, flush async actions */
	del_timer_sync(&q->backing_dev_info->laptop_mode_wb_timer);
	blk_sync_queue(q);

	/*
	 * I/O scheduler exit is only safe after the sysfs scheduler attribute
	 * has been removed.
	 */
	WARN_ON_ONCE(q->kobj.state_in_sysfs);

	/*
	 * Since the I/O scheduler exit code may access cgroup information,
	 * perform I/O scheduler exit before disassociating from the block
	 * cgroup controller.
	 */
	if (q->elevator) {
		ioc_clear_queue(q);
		elevator_exit(q, q->elevator);
		q->elevator = NULL;
	}

	/*
	 * Remove all references to @q from the block cgroup controller before
	 * restoring @q->queue_lock to avoid that restoring this pointer causes
	 * e.g. blkcg_print_blkgs() to crash.
	 */
	blkcg_exit_queue(q);

	/*
	 * Since the cgroup code may dereference the @q->backing_dev_info
	 * pointer, only decrease its reference count after having removed the
	 * association with the block cgroup controller.
	 */
	bdi_put(q->backing_dev_info);

	if (q->mq_ops)
		blk_mq_free_queue(q);
	percpu_ref_exit(&q->q_usage_counter);

	spin_lock_irq(lock);
	if (q->queue_lock != &q->__queue_lock)
		q->queue_lock = &q->__queue_lock;
	spin_unlock_irq(lock);

	/* @q is and will stay empty, shutdown and put */
	blk_put_queue(q);
}
