struct request_queue *blk_alloc_queue_node(gfp_t gfp_mask, int node_id,
					   spinlock_t *lock)
{
	struct request_queue *q;
	int ret;

	q = kmem_cache_alloc_node(blk_requestq_cachep,
				gfp_mask | __GFP_ZERO, node_id);
	if (!q)
		return NULL;

	INIT_LIST_HEAD(&q->queue_head);
	q->last_merge = NULL;
	q->end_sector = 0;
	q->boundary_rq = NULL;

	q->id = ida_simple_get(&blk_queue_ida, 0, 0, gfp_mask);
	if (q->id < 0)
		goto fail_q;

	ret = bioset_init(&q->bio_split, BIO_POOL_SIZE, 0, BIOSET_NEED_BVECS);
	if (ret)
		goto fail_id;

	q->backing_dev_info = bdi_alloc_node(gfp_mask, node_id);
	if (!q->backing_dev_info)
		goto fail_split;

	q->stats = blk_alloc_queue_stats();
	if (!q->stats)
		goto fail_stats;

	q->backing_dev_info->ra_pages =
			(VM_MAX_READAHEAD * 1024) / PAGE_SIZE;
	q->backing_dev_info->capabilities = BDI_CAP_CGROUP_WRITEBACK;
	q->backing_dev_info->name = "block";
	q->node = node_id;

	timer_setup(&q->backing_dev_info->laptop_mode_wb_timer,
		    laptop_mode_timer_fn, 0);
	timer_setup(&q->timeout, blk_rq_timed_out_timer, 0);
	INIT_WORK(&q->timeout_work, NULL);
	INIT_LIST_HEAD(&q->queue_head);
	INIT_LIST_HEAD(&q->timeout_list);
	INIT_LIST_HEAD(&q->icq_list);
#ifdef CONFIG_BLK_CGROUP
	INIT_LIST_HEAD(&q->blkg_list);
#endif
	INIT_DELAYED_WORK(&q->delay_work, blk_delay_work);

	kobject_init(&q->kobj, &blk_queue_ktype);

#ifdef CONFIG_BLK_DEV_IO_TRACE
	mutex_init(&q->blk_trace_mutex);
#endif
	mutex_init(&q->sysfs_lock);
	spin_lock_init(&q->__queue_lock);

	if (!q->mq_ops)
		q->queue_lock = lock ? : &q->__queue_lock;

	/*
	 * A queue starts its life with bypass turned on to avoid
	 * unnecessary bypass on/off overhead and nasty surprises during
	 * init.  The initial bypass will be finished when the queue is
	 * registered by blk_register_queue().
	 */
	q->bypass_depth = 1;
	queue_flag_set_unlocked(QUEUE_FLAG_BYPASS, q);

	init_waitqueue_head(&q->mq_freeze_wq);

	/*
	 * Init percpu_ref in atomic mode so that it's faster to shutdown.
	 * See blk_register_queue() for details.
	 */
	if (percpu_ref_init(&q->q_usage_counter,
				blk_queue_usage_counter_release,
				PERCPU_REF_INIT_ATOMIC, GFP_KERNEL))
		goto fail_bdi;

	if (blkcg_init_queue(q))
		goto fail_ref;

	return q;

fail_ref:
	percpu_ref_exit(&q->q_usage_counter);
fail_bdi:
	blk_free_queue_stats(q->stats);
fail_stats:
	bdi_put(q->backing_dev_info);
fail_split:
	bioset_exit(&q->bio_split);
fail_id:
	ida_simple_remove(&blk_queue_ida, q->id);
fail_q:
	kmem_cache_free(blk_requestq_cachep, q);
	return NULL;
}
