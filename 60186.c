int fscrypt_has_permitted_context(struct inode *parent, struct inode *child)
{
	struct fscrypt_info *parent_ci, *child_ci;
	int res;

	if ((parent == NULL) || (child == NULL)) {
		printk(KERN_ERR	"parent %p child %p\n", parent, child);
		BUG_ON(1);
	}

	/* no restrictions if the parent directory is not encrypted */
	if (!parent->i_sb->s_cop->is_encrypted(parent))
		return 1;
	/* if the child directory is not encrypted, this is always a problem */
	if (!parent->i_sb->s_cop->is_encrypted(child))
		return 0;
	res = fscrypt_get_encryption_info(parent);
	if (res)
		return 0;
	res = fscrypt_get_encryption_info(child);
	if (res)
		return 0;
	parent_ci = parent->i_crypt_info;
	child_ci = child->i_crypt_info;
	if (!parent_ci && !child_ci)
		return 1;
	if (!parent_ci || !child_ci)
		return 0;

	return (memcmp(parent_ci->ci_master_key,
			child_ci->ci_master_key,
			FS_KEY_DESCRIPTOR_SIZE) == 0 &&
		(parent_ci->ci_data_mode == child_ci->ci_data_mode) &&
		(parent_ci->ci_filename_mode == child_ci->ci_filename_mode) &&
		(parent_ci->ci_flags == child_ci->ci_flags));
}
