static struct dentry *ovl_upperdentry_dereference(struct ovl_entry *oe)
{
	struct dentry *upperdentry = ACCESS_ONCE(oe->__upperdentry);
	/*
	 * Make sure to order reads to upperdentry wrt ovl_dentry_update()
	 */
	smp_read_barrier_depends();
	return upperdentry;
}
