lo_do_transfer(struct loop_device *lo, int cmd,
	       struct page *rpage, unsigned roffs,
	       struct page *lpage, unsigned loffs,
	       int size, sector_t rblock)
{
	int ret;

	ret = lo->transfer(lo, cmd, rpage, roffs, lpage, loffs, size, rblock);
	if (likely(!ret))
		return 0;

	printk_ratelimited(KERN_ERR
		"loop: Transfer error at byte offset %llu, length %i.\n",
		(unsigned long long)rblock << 9, size);
	return ret;
}
