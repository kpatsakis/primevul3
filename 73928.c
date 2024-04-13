static int kill_ioctx(struct mm_struct *mm, struct kioctx *ctx,
		struct completion *requests_done)
{
	struct kioctx_table *table;

	if (atomic_xchg(&ctx->dead, 1))
		return -EINVAL;


	spin_lock(&mm->ioctx_lock);
	table = rcu_dereference_raw(mm->ioctx_table);
	WARN_ON(ctx != table->table[ctx->id]);
	table->table[ctx->id] = NULL;
	spin_unlock(&mm->ioctx_lock);

	/* percpu_ref_kill() will do the necessary call_rcu() */
	wake_up_all(&ctx->wait);

	/*
	 * It'd be more correct to do this in free_ioctx(), after all
	 * the outstanding kiocbs have finished - but by then io_destroy
	 * has already returned, so io_setup() could potentially return
	 * -EAGAIN with no ioctxs actually in use (as far as userspace
	 *  could tell).
	 */
	aio_nr_sub(ctx->max_reqs);

	if (ctx->mmap_size)
		vm_munmap(ctx->mmap_base, ctx->mmap_size);

	ctx->requests_done = requests_done;
	percpu_ref_kill(&ctx->users);
	return 0;
}
