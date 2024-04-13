static void __exit loop_exit(void)
{
	unsigned long range;

	range = max_loop ? max_loop << part_shift : 1UL << MINORBITS;

	idr_for_each(&loop_index_idr, &loop_exit_cb, NULL);
	idr_destroy(&loop_index_idr);

	blk_unregister_region(MKDEV(LOOP_MAJOR, 0), range);
	unregister_blkdev(LOOP_MAJOR, "loop");

	misc_deregister(&loop_misc);
}
