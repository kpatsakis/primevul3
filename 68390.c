static int lo_read_simple(struct loop_device *lo, struct request *rq,
		loff_t pos)
{
	struct bio_vec bvec;
	struct req_iterator iter;
	struct iov_iter i;
	ssize_t len;

	rq_for_each_segment(bvec, rq, iter) {
		iov_iter_bvec(&i, ITER_BVEC, &bvec, 1, bvec.bv_len);
		len = vfs_iter_read(lo->lo_backing_file, &i, &pos, 0);
		if (len < 0)
			return len;

		flush_dcache_page(bvec.bv_page);

		if (len != bvec.bv_len) {
			struct bio *bio;

			__rq_for_each_bio(bio, rq)
				zero_fill_bio(bio);
			break;
		}
		cond_resched();
	}

	return 0;
}
