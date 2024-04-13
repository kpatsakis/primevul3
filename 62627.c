static void ovl_dentry_release(struct dentry *dentry)
{
	struct ovl_entry *oe = dentry->d_fsdata;

	if (oe) {
		dput(oe->__upperdentry);
		dput(oe->lowerdentry);
		kfree_rcu(oe, rcu);
	}
}
