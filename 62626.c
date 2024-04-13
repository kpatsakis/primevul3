struct dentry *ovl_dentry_real(struct dentry *dentry)
{
	struct ovl_entry *oe = dentry->d_fsdata;
	struct dentry *realdentry;

	realdentry = ovl_upperdentry_dereference(oe);
	if (!realdentry)
		realdentry = oe->lowerdentry;

	return realdentry;
}
