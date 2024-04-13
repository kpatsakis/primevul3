void exit_aio(struct mm_struct *mm)
{
	struct kioctx_table *table = rcu_dereference_raw(mm->ioctx_table);
	int i;

	if (!table)
		return;

	for (i = 0; i < table->nr; ++i) {
		struct kioctx *ctx = table->table[i];
		struct completion requests_done =
			COMPLETION_INITIALIZER_ONSTACK(requests_done);

		if (!ctx)
			continue;
		/*
		 * We don't need to bother with munmap() here - exit_mmap(mm)
		 * is coming and it'll unmap everything. And we simply can't,
		 * this is not necessarily our ->mm.
		 * Since kill_ioctx() uses non-zero ->mmap_size as indicator
		 * that it needs to unmap the area, just set it to 0.
		 */
		ctx->mmap_size = 0;
		kill_ioctx(mm, ctx, &requests_done);

		/* Wait until all IO for the context are done. */
		wait_for_completion(&requests_done);
	}

	RCU_INIT_POINTER(mm->ioctx_table, NULL);
	kfree(table);
}
