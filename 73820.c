void d_rehash(struct dentry * entry)
{
	spin_lock(&entry->d_lock);
	_d_rehash(entry);
	spin_unlock(&entry->d_lock);
}
