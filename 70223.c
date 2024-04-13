xfs_dentry_to_name(
	struct xfs_name	*namep,
	struct dentry	*dentry)
{
	namep->name = dentry->d_name.name;
	namep->len = dentry->d_name.len;
	namep->type = XFS_DIR3_FT_UNKNOWN;
}
