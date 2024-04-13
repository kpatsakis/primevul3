static void f2fs_submit_discard_endio(struct bio *bio)
{
	struct discard_cmd *dc = (struct discard_cmd *)bio->bi_private;

	dc->error = blk_status_to_errno(bio->bi_status);
	dc->state = D_DONE;
	complete_all(&dc->wait);
	bio_put(bio);
}
