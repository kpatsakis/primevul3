static int lo_rw_aio(struct loop_device *lo, struct loop_cmd *cmd,
		     loff_t pos, bool rw)
{
	struct iov_iter iter;
	struct bio_vec *bvec;
	struct request *rq = cmd->rq;
	struct bio *bio = rq->bio;
	struct file *file = lo->lo_backing_file;
	unsigned int offset;
	int segments = 0;
	int ret;

	if (rq->bio != rq->biotail) {
		struct req_iterator iter;
		struct bio_vec tmp;

		__rq_for_each_bio(bio, rq)
			segments += bio_segments(bio);
		bvec = kmalloc(sizeof(struct bio_vec) * segments, GFP_NOIO);
		if (!bvec)
			return -EIO;
		cmd->bvec = bvec;

		/*
		 * The bios of the request may be started from the middle of
		 * the 'bvec' because of bio splitting, so we can't directly
		 * copy bio->bi_iov_vec to new bvec. The rq_for_each_segment
		 * API will take care of all details for us.
		 */
		rq_for_each_segment(tmp, rq, iter) {
			*bvec = tmp;
			bvec++;
		}
		bvec = cmd->bvec;
		offset = 0;
	} else {
		/*
		 * Same here, this bio may be started from the middle of the
		 * 'bvec' because of bio splitting, so offset from the bvec
		 * must be passed to iov iterator
		 */
		offset = bio->bi_iter.bi_bvec_done;
		bvec = __bvec_iter_bvec(bio->bi_io_vec, bio->bi_iter);
		segments = bio_segments(bio);
	}
	atomic_set(&cmd->ref, 2);

	iov_iter_bvec(&iter, ITER_BVEC | rw, bvec,
		      segments, blk_rq_bytes(rq));
	iter.iov_offset = offset;

	cmd->iocb.ki_pos = pos;
	cmd->iocb.ki_filp = file;
	cmd->iocb.ki_complete = lo_rw_aio_complete;
	cmd->iocb.ki_flags = IOCB_DIRECT;
	if (cmd->css)
		kthread_associate_blkcg(cmd->css);

	if (rw == WRITE)
		ret = call_write_iter(file, &cmd->iocb, &iter);
	else
		ret = call_read_iter(file, &cmd->iocb, &iter);

	lo_rw_aio_do_completion(cmd);
	kthread_associate_blkcg(NULL);

	if (ret != -EIOCBQUEUED)
		cmd->iocb.ki_complete(&cmd->iocb, ret, 0);
	return 0;
}
