xfs_vn_get_link(
	struct dentry		*dentry,
	struct inode		*inode,
	struct delayed_call	*done)
{
	char			*link;
	int			error = -ENOMEM;

	if (!dentry)
		return ERR_PTR(-ECHILD);

	link = kmalloc(XFS_SYMLINK_MAXLEN+1, GFP_KERNEL);
	if (!link)
		goto out_err;

	error = xfs_readlink(XFS_I(d_inode(dentry)), link);
	if (unlikely(error))
		goto out_kfree;

	set_delayed_call(done, kfree_link, link);
	return link;

 out_kfree:
	kfree(link);
 out_err:
	return ERR_PTR(error);
}
