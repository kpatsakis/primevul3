static void dentry_lru_add(struct dentry *dentry)
{
	if (unlikely(!(dentry->d_flags & DCACHE_LRU_LIST)))
		d_lru_add(dentry);
}
