static int f2fs_quota_on(struct super_block *sb, int type, int format_id,
							const struct path *path)
{
	struct inode *inode;
	int err;

	err = f2fs_quota_sync(sb, type);
	if (err)
		return err;

	err = dquot_quota_on(sb, type, format_id, path);
	if (err)
		return err;

	inode = d_inode(path->dentry);

	inode_lock(inode);
	F2FS_I(inode)->i_flags |= FS_NOATIME_FL | FS_IMMUTABLE_FL;
	inode_set_flags(inode, S_NOATIME | S_IMMUTABLE,
					S_NOATIME | S_IMMUTABLE);
	inode_unlock(inode);
	f2fs_mark_inode_dirty_sync(inode, false);

	return 0;
}
