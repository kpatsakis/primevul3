static struct dentry *nsfs_mount(struct file_system_type *fs_type,
			int flags, const char *dev_name, void *data)
{
	return mount_pseudo(fs_type, "nsfs:", &nsfs_ops,
			&ns_dentry_operations, NSFS_MAGIC);
}
