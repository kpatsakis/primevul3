static int f2fs_statfs(struct dentry *dentry, struct kstatfs *buf)
{
	struct super_block *sb = dentry->d_sb;
	struct f2fs_sb_info *sbi = F2FS_SB(sb);
	u64 id = huge_encode_dev(sb->s_bdev->bd_dev);
	block_t total_count, user_block_count, start_count, ovp_count;
	u64 avail_node_count;

	total_count = le64_to_cpu(sbi->raw_super->block_count);
	user_block_count = sbi->user_block_count;
	start_count = le32_to_cpu(sbi->raw_super->segment0_blkaddr);
	ovp_count = SM_I(sbi)->ovp_segments << sbi->log_blocks_per_seg;
	buf->f_type = F2FS_SUPER_MAGIC;
	buf->f_bsize = sbi->blocksize;

	buf->f_blocks = total_count - start_count;
	buf->f_bfree = user_block_count - valid_user_blocks(sbi) + ovp_count;
	buf->f_bavail = user_block_count - valid_user_blocks(sbi) -
						sbi->reserved_blocks;

	avail_node_count = sbi->total_node_count - F2FS_RESERVED_NODE_NUM;

	if (avail_node_count > user_block_count) {
		buf->f_files = user_block_count;
		buf->f_ffree = buf->f_bavail;
	} else {
		buf->f_files = avail_node_count;
		buf->f_ffree = min(avail_node_count - valid_node_count(sbi),
					buf->f_bavail);
	}

	buf->f_namelen = F2FS_NAME_LEN;
	buf->f_fsid.val[0] = (u32)id;
	buf->f_fsid.val[1] = (u32)(id >> 32);

#ifdef CONFIG_QUOTA
	if (is_inode_flag_set(dentry->d_inode, FI_PROJ_INHERIT) &&
			sb_has_quota_limits_enabled(sb, PRJQUOTA)) {
		f2fs_statfs_project(sb, F2FS_I(dentry->d_inode)->i_projid, buf);
	}
#endif
	return 0;
}
