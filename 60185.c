int fscrypt_get_policy(struct inode *inode, struct fscrypt_policy *policy)
{
	struct fscrypt_context ctx;
	int res;

	if (!inode->i_sb->s_cop->get_context ||
			!inode->i_sb->s_cop->is_encrypted(inode))
		return -ENODATA;

	res = inode->i_sb->s_cop->get_context(inode, &ctx, sizeof(ctx));
	if (res != sizeof(ctx))
		return -ENODATA;
	if (ctx.format != FS_ENCRYPTION_CONTEXT_FORMAT_V1)
		return -EINVAL;

	policy->version = 0;
	policy->contents_encryption_mode = ctx.contents_encryption_mode;
	policy->filenames_encryption_mode = ctx.filenames_encryption_mode;
	policy->flags = ctx.flags;
	memcpy(&policy->master_key_descriptor, ctx.master_key_descriptor,
				FS_KEY_DESCRIPTOR_SIZE);
	return 0;
}
