static void nsfs_evict(struct inode *inode)
{
	struct ns_common *ns = inode->i_private;
	clear_inode(inode);
	ns->ops->put(ns);
}
