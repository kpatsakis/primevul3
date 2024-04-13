void f2fs_quota_off_umount(struct super_block *sb)
{
	int type;

	for (type = 0; type < MAXQUOTAS; type++)
		f2fs_quota_off(sb, type);
}
