void ovl_dentry_set_opaque(struct dentry *dentry, bool opaque)
{
	struct ovl_entry *oe = dentry->d_fsdata;
	oe->opaque = opaque;
}
