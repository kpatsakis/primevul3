static int ecryptfs_init_lower_file(struct dentry *dentry,
				    struct file **lower_file)
{
	const struct cred *cred = current_cred();
	struct path *path = ecryptfs_dentry_to_lower_path(dentry);
	int rc;

	rc = ecryptfs_privileged_open(lower_file, path->dentry, path->mnt,
				      cred);
	if (rc) {
		printk(KERN_ERR "Error opening lower file "
		       "for lower_dentry [0x%p] and lower_mnt [0x%p]; "
		       "rc = [%d]\n", path->dentry, path->mnt, rc);
		(*lower_file) = NULL;
	}
	return rc;
}
