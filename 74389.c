void fuse_abort_conn(struct fuse_conn *fc, bool is_abort)
{
	struct fuse_iqueue *fiq = &fc->iq;

	spin_lock(&fc->lock);
	if (fc->connected) {
		struct fuse_dev *fud;
		struct fuse_req *req, *next;
		LIST_HEAD(to_end);
		unsigned int i;

		/* Background queuing checks fc->connected under bg_lock */
		spin_lock(&fc->bg_lock);
		fc->connected = 0;
		spin_unlock(&fc->bg_lock);

		fc->aborted = is_abort;
		fuse_set_initialized(fc);
		list_for_each_entry(fud, &fc->devices, entry) {
			struct fuse_pqueue *fpq = &fud->pq;

			spin_lock(&fpq->lock);
			fpq->connected = 0;
			list_for_each_entry_safe(req, next, &fpq->io, list) {
				req->out.h.error = -ECONNABORTED;
				spin_lock(&req->waitq.lock);
				set_bit(FR_ABORTED, &req->flags);
				if (!test_bit(FR_LOCKED, &req->flags)) {
					set_bit(FR_PRIVATE, &req->flags);
					__fuse_get_request(req);
					list_move(&req->list, &to_end);
				}
				spin_unlock(&req->waitq.lock);
			}
			for (i = 0; i < FUSE_PQ_HASH_SIZE; i++)
				list_splice_tail_init(&fpq->processing[i],
						      &to_end);
			spin_unlock(&fpq->lock);
		}
		spin_lock(&fc->bg_lock);
		fc->blocked = 0;
		fc->max_background = UINT_MAX;
		flush_bg_queue(fc);
		spin_unlock(&fc->bg_lock);

		spin_lock(&fiq->waitq.lock);
		fiq->connected = 0;
		list_for_each_entry(req, &fiq->pending, list)
			clear_bit(FR_PENDING, &req->flags);
		list_splice_tail_init(&fiq->pending, &to_end);
		while (forget_pending(fiq))
			kfree(dequeue_forget(fiq, 1, NULL));
		wake_up_all_locked(&fiq->waitq);
		spin_unlock(&fiq->waitq.lock);
		kill_fasync(&fiq->fasync, SIGIO, POLL_IN);
		end_polls(fc);
		wake_up_all(&fc->blocked_waitq);
		spin_unlock(&fc->lock);

		end_requests(fc, &to_end);
	} else {
		spin_unlock(&fc->lock);
	}
}
