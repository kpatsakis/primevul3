void build_free_nids(struct f2fs_sb_info *sbi, bool sync, bool mount)
{
	mutex_lock(&NM_I(sbi)->build_lock);
	__build_free_nids(sbi, sync, mount);
	mutex_unlock(&NM_I(sbi)->build_lock);
}
