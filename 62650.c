void ovl_set_dir_cache(struct dentry *dentry, struct ovl_dir_cache *cache)
{
	struct ovl_entry *oe = dentry->d_fsdata;

	oe->cache = cache;
}
