struct ovl_dir_cache *ovl_dir_cache(struct dentry *dentry)
{
	struct ovl_entry *oe = dentry->d_fsdata;

	return oe->cache;
}
