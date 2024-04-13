static inline struct hlist_bl_head *d_hash(const struct dentry *parent,
					unsigned int hash)
{
	hash += (unsigned long) parent / L1_CACHE_BYTES;
	return dentry_hashtable + hash_32(hash, d_hash_shift);
}
