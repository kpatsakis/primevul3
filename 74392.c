int fuse_dev_release(struct inode *inode, struct file *file)
{
	struct fuse_dev *fud = fuse_get_dev(file);

	if (fud) {
		struct fuse_conn *fc = fud->fc;
		struct fuse_pqueue *fpq = &fud->pq;
		LIST_HEAD(to_end);
		unsigned int i;

		spin_lock(&fpq->lock);
		WARN_ON(!list_empty(&fpq->io));
		for (i = 0; i < FUSE_PQ_HASH_SIZE; i++)
			list_splice_init(&fpq->processing[i], &to_end);
		spin_unlock(&fpq->lock);

		end_requests(fc, &to_end);

		/* Are we the last open device? */
		if (atomic_dec_and_test(&fc->dev_count)) {
			WARN_ON(fc->iq.fasync != NULL);
			fuse_abort_conn(fc, false);
		}
		fuse_dev_free(fud);
	}
	return 0;
}
