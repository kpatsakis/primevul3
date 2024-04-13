static ssize_t fuse_dev_do_write(struct fuse_dev *fud,
				 struct fuse_copy_state *cs, size_t nbytes)
{
	int err;
	struct fuse_conn *fc = fud->fc;
	struct fuse_pqueue *fpq = &fud->pq;
	struct fuse_req *req;
	struct fuse_out_header oh;

	if (nbytes < sizeof(struct fuse_out_header))
		return -EINVAL;

	err = fuse_copy_one(cs, &oh, sizeof(oh));
	if (err)
		goto err_finish;

	err = -EINVAL;
	if (oh.len != nbytes)
		goto err_finish;

	/*
	 * Zero oh.unique indicates unsolicited notification message
	 * and error contains notification code.
	 */
	if (!oh.unique) {
		err = fuse_notify(fc, oh.error, nbytes - sizeof(oh), cs);
		return err ? err : nbytes;
	}

	err = -EINVAL;
	if (oh.error <= -1000 || oh.error > 0)
		goto err_finish;

	spin_lock(&fpq->lock);
	err = -ENOENT;
	if (!fpq->connected)
		goto err_unlock_pq;

	req = request_find(fpq, oh.unique & ~FUSE_INT_REQ_BIT);
	if (!req)
		goto err_unlock_pq;

	/* Is it an interrupt reply ID? */
	if (oh.unique & FUSE_INT_REQ_BIT) {
		__fuse_get_request(req);
		spin_unlock(&fpq->lock);

		err = -EINVAL;
		if (nbytes != sizeof(struct fuse_out_header)) {
			fuse_put_request(fc, req);
			goto err_finish;
		}

		if (oh.error == -ENOSYS)
			fc->no_interrupt = 1;
		else if (oh.error == -EAGAIN)
			queue_interrupt(&fc->iq, req);
		fuse_put_request(fc, req);

		fuse_copy_finish(cs);
		return nbytes;
	}

	clear_bit(FR_SENT, &req->flags);
	list_move(&req->list, &fpq->io);
	req->out.h = oh;
	set_bit(FR_LOCKED, &req->flags);
	spin_unlock(&fpq->lock);
	cs->req = req;
	if (!req->out.page_replace)
		cs->move_pages = 0;

	err = copy_out_args(cs, &req->out, nbytes);
	fuse_copy_finish(cs);

	spin_lock(&fpq->lock);
	clear_bit(FR_LOCKED, &req->flags);
	if (!fpq->connected)
		err = -ENOENT;
	else if (err)
		req->out.h.error = -EIO;
	if (!test_bit(FR_PRIVATE, &req->flags))
		list_del_init(&req->list);
	spin_unlock(&fpq->lock);

	request_end(fc, req);

	return err ? err : nbytes;

 err_unlock_pq:
	spin_unlock(&fpq->lock);
 err_finish:
	fuse_copy_finish(cs);
	return err;
}
