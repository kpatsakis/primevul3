static int lo_write_simple(struct loop_device *lo, struct request *rq,
		loff_t pos)
{
	struct bio_vec bvec;
	struct req_iterator iter;
	int ret = 0;

	rq_for_each_segment(bvec, rq, iter) {
		ret = lo_write_bvec(lo->lo_backing_file, &bvec, &pos);
		if (ret < 0)
			break;
		cond_resched();
	}

	return ret;
}
