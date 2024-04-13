static int f2fs_quota_off(struct super_block *sb, int type)
{
	struct inode *inode = sb_dqopt(sb)->files[type];
	int err;

	if (!inode || !igrab(inode))
		return dquot_quota_off(sb, type);

	f2fs_quota_sync(sb, type);

	err = dquot_quota_off(sb, type);
	if (err)
		goto out_put;

	inode_lock(inode);
	F2FS_I(inode)->i_flags &= ~(FS_NOATIME_FL | FS_IMMUTABLE_FL);
	inode_set_flags(inode, 0, S_NOATIME | S_IMMUTABLE);
	inode_unlock(inode);
	f2fs_mark_inode_dirty_sync(inode, false);
out_put:
	iput(inode);
	return err;
}
