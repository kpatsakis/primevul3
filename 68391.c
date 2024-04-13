static int lo_read_transfer(struct loop_device *lo, struct request *rq,
		loff_t pos)
{
	struct bio_vec bvec, b;
	struct req_iterator iter;
	struct iov_iter i;
	struct page *page;
	ssize_t len;
	int ret = 0;

	page = alloc_page(GFP_NOIO);
	if (unlikely(!page))
		return -ENOMEM;

	rq_for_each_segment(bvec, rq, iter) {
		loff_t offset = pos;

		b.bv_page = page;
		b.bv_offset = 0;
		b.bv_len = bvec.bv_len;

		iov_iter_bvec(&i, ITER_BVEC, &b, 1, b.bv_len);
		len = vfs_iter_read(lo->lo_backing_file, &i, &pos, 0);
		if (len < 0) {
			ret = len;
			goto out_free_page;
		}

		ret = lo_do_transfer(lo, READ, page, 0, bvec.bv_page,
			bvec.bv_offset, len, offset >> 9);
		if (ret)
			goto out_free_page;

		flush_dcache_page(bvec.bv_page);

		if (len != bvec.bv_len) {
			struct bio *bio;

			__rq_for_each_bio(bio, rq)
				zero_fill_bio(bio);
			break;
		}
	}

	ret = 0;
out_free_page:
	__free_page(page);
	return ret;
}
