static struct inode *f2fs_alloc_inode(struct super_block *sb)
{
	struct f2fs_inode_info *fi;

	fi = kmem_cache_alloc(f2fs_inode_cachep, GFP_F2FS_ZERO);
	if (!fi)
		return NULL;

	init_once((void *) fi);

	/* Initialize f2fs-specific inode info */
	fi->vfs_inode.i_version = 1;
	atomic_set(&fi->dirty_pages, 0);
	fi->i_current_depth = 1;
	fi->i_advise = 0;
	init_rwsem(&fi->i_sem);
	INIT_LIST_HEAD(&fi->dirty_list);
	INIT_LIST_HEAD(&fi->gdirty_list);
	INIT_LIST_HEAD(&fi->inmem_pages);
	mutex_init(&fi->inmem_lock);
	init_rwsem(&fi->dio_rwsem[READ]);
	init_rwsem(&fi->dio_rwsem[WRITE]);
	init_rwsem(&fi->i_mmap_sem);
	init_rwsem(&fi->i_xattr_sem);

#ifdef CONFIG_QUOTA
	memset(&fi->i_dquot, 0, sizeof(fi->i_dquot));
	fi->i_reserved_quota = 0;
#endif
	/* Will be used by directory only */
	fi->i_dir_level = F2FS_SB(sb)->dir_level;

	return &fi->vfs_inode;
}
