int blk_rq_map_user_iov(struct request_queue *q, struct request *rq,
			struct rq_map_data *map_data,
			const struct iov_iter *iter, gfp_t gfp_mask)
{
	bool copy = false;
	unsigned long align = q->dma_pad_mask | queue_dma_alignment(q);
	struct bio *bio = NULL;
	struct iov_iter i;
	int ret = -EINVAL;

	if (!iter_is_iovec(iter))
		goto fail;

	if (map_data)
		copy = true;
	else if (blk_queue_may_bounce(q))
		copy = true;
	else if (iov_iter_alignment(iter) & align)
		copy = true;
	else if (queue_virt_boundary(q))
		copy = queue_virt_boundary(q) & iov_iter_gap_alignment(iter);

	i = *iter;
	do {
		if (copy)
			ret = bio_copy_user_iov(rq, map_data, &i, gfp_mask);
		else
			ret = bio_map_user_iov(rq, &i, gfp_mask);
		if (ret)
			goto unmap_rq;
		if (!bio)
			bio = rq->bio;
	} while (iov_iter_count(&i));

	return 0;

unmap_rq:
	blk_rq_unmap_user(bio);
fail:
	rq->bio = NULL;
	return ret;
}