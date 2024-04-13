static int is_encryption_context_consistent_with_policy(struct inode *inode,
				const struct fscrypt_policy *policy)
{
	struct fscrypt_context ctx;
	int res;

	if (!inode->i_sb->s_cop->get_context)
		return 0;

	res = inode->i_sb->s_cop->get_context(inode, &ctx, sizeof(ctx));
	if (res != sizeof(ctx))
		return 0;

	return (memcmp(ctx.master_key_descriptor, policy->master_key_descriptor,
			FS_KEY_DESCRIPTOR_SIZE) == 0 &&
			(ctx.flags == policy->flags) &&
			(ctx.contents_encryption_mode ==
			 policy->contents_encryption_mode) &&
			(ctx.filenames_encryption_mode ==
			 policy->filenames_encryption_mode));
}
