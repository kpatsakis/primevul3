struct page *new_inode_page(struct inode *inode)
{
	struct dnode_of_data dn;

	/* allocate inode page for new inode */
	set_new_dnode(&dn, inode, NULL, NULL, inode->i_ino);

	/* caller should f2fs_put_page(page, 1); */
	return new_node_page(&dn, 0, NULL);
}
