static void request_end(struct fuse_conn *fc, struct fuse_req *req)
{
	struct fuse_iqueue *fiq = &fc->iq;

	if (test_and_set_bit(FR_FINISHED, &req->flags))
		goto put_request;

	spin_lock(&fiq->waitq.lock);
	list_del_init(&req->intr_entry);
	spin_unlock(&fiq->waitq.lock);
	WARN_ON(test_bit(FR_PENDING, &req->flags));
	WARN_ON(test_bit(FR_SENT, &req->flags));
	if (test_bit(FR_BACKGROUND, &req->flags)) {
		spin_lock(&fc->bg_lock);
		clear_bit(FR_BACKGROUND, &req->flags);
		if (fc->num_background == fc->max_background) {
			fc->blocked = 0;
			wake_up(&fc->blocked_waitq);
		} else if (!fc->blocked) {
			/*
			 * Wake up next waiter, if any.  It's okay to use
			 * waitqueue_active(), as we've already synced up
			 * fc->blocked with waiters with the wake_up() call
			 * above.
			 */
			if (waitqueue_active(&fc->blocked_waitq))
				wake_up(&fc->blocked_waitq);
		}

		if (fc->num_background == fc->congestion_threshold && fc->sb) {
			clear_bdi_congested(fc->sb->s_bdi, BLK_RW_SYNC);
			clear_bdi_congested(fc->sb->s_bdi, BLK_RW_ASYNC);
		}
		fc->num_background--;
		fc->active_background--;
		flush_bg_queue(fc);
		spin_unlock(&fc->bg_lock);
	}
	wake_up(&req->waitq);
	if (req->end)
		req->end(fc, req);
put_request:
	fuse_put_request(fc, req);
}
