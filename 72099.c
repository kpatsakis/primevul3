struct request *blk_peek_request(struct request_queue *q)
{
	struct request *rq;
	int ret;

	lockdep_assert_held(q->queue_lock);
	WARN_ON_ONCE(q->mq_ops);

	while ((rq = elv_next_request(q)) != NULL) {
		if (!(rq->rq_flags & RQF_STARTED)) {
			/*
			 * This is the first time the device driver
			 * sees this request (possibly after
			 * requeueing).  Notify IO scheduler.
			 */
			if (rq->rq_flags & RQF_SORTED)
				elv_activate_rq(q, rq);

			/*
			 * just mark as started even if we don't start
			 * it, a request that has been delayed should
			 * not be passed by new incoming requests
			 */
			rq->rq_flags |= RQF_STARTED;
			trace_block_rq_issue(q, rq);
		}

		if (!q->boundary_rq || q->boundary_rq == rq) {
			q->end_sector = rq_end_sector(rq);
			q->boundary_rq = NULL;
		}

		if (rq->rq_flags & RQF_DONTPREP)
			break;

		if (q->dma_drain_size && blk_rq_bytes(rq)) {
			/*
			 * make sure space for the drain appears we
			 * know we can do this because max_hw_segments
			 * has been adjusted to be one fewer than the
			 * device can handle
			 */
			rq->nr_phys_segments++;
		}

		if (!q->prep_rq_fn)
			break;

		ret = q->prep_rq_fn(q, rq);
		if (ret == BLKPREP_OK) {
			break;
		} else if (ret == BLKPREP_DEFER) {
			/*
			 * the request may have been (partially) prepped.
			 * we need to keep this request in the front to
			 * avoid resource deadlock.  RQF_STARTED will
			 * prevent other fs requests from passing this one.
			 */
			if (q->dma_drain_size && blk_rq_bytes(rq) &&
			    !(rq->rq_flags & RQF_DONTPREP)) {
				/*
				 * remove the space for the drain we added
				 * so that we don't add it again
				 */
				--rq->nr_phys_segments;
			}

			rq = NULL;
			break;
		} else if (ret == BLKPREP_KILL || ret == BLKPREP_INVALID) {
			rq->rq_flags |= RQF_QUIET;
			/*
			 * Mark this request as started so we don't trigger
			 * any debug logic in the end I/O path.
			 */
			blk_start_request(rq);
			__blk_end_request_all(rq, ret == BLKPREP_INVALID ?
					BLK_STS_TARGET : BLK_STS_IOERR);
		} else {
			printk(KERN_ERR "%s: bad return=%d\n", __func__, ret);
			break;
		}
	}

	return rq;
}
