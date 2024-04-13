static void ovl_put_super(struct super_block *sb)
{
	struct ovl_fs *ufs = sb->s_fs_info;

	dput(ufs->workdir);
	mntput(ufs->upper_mnt);
	mntput(ufs->lower_mnt);

	kfree(ufs->config.lowerdir);
	kfree(ufs->config.upperdir);
	kfree(ufs->config.workdir);
	kfree(ufs);
}
