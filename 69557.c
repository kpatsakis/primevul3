static void blk_mq_free_bitmap(struct blk_mq_ctxmap *bitmap)
{
	kfree(bitmap->map);
}
