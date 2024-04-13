void f2fs_update_extent_cache(struct dnode_of_data *dn)
{
	pgoff_t fofs;
	block_t blkaddr;

	if (!f2fs_may_extent_tree(dn->inode))
		return;

	if (dn->data_blkaddr == NEW_ADDR)
		blkaddr = NULL_ADDR;
	else
		blkaddr = dn->data_blkaddr;

	fofs = start_bidx_of_node(ofs_of_node(dn->node_page), dn->inode) +
								dn->ofs_in_node;
	f2fs_update_extent_tree_range(dn->inode, fofs, blkaddr, 1);
}
