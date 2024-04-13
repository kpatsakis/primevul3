static bool ovl_is_allowed_fs_type(struct dentry *root)
{
	const struct dentry_operations *dop = root->d_op;

	/*
	 * We don't support:
	 *  - automount filesystems
	 *  - filesystems with revalidate (FIXME for lower layer)
	 *  - filesystems with case insensitive names
	 */
	if (dop &&
	    (dop->d_manage || dop->d_automount ||
	     dop->d_revalidate || dop->d_weak_revalidate ||
	     dop->d_compare || dop->d_hash)) {
		return false;
	}
	return true;
}
