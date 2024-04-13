static void truncate_node(struct dnode_of_data *dn)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(dn->inode);
	struct node_info ni;

	get_node_info(sbi, dn->nid, &ni);
	if (dn->inode->i_blocks == 0) {
		f2fs_bug_on(sbi, ni.blk_addr != NULL_ADDR);
		goto invalidate;
	}
	f2fs_bug_on(sbi, ni.blk_addr == NULL_ADDR);

	/* Deallocate node address */
	invalidate_blocks(sbi, ni.blk_addr);
	dec_valid_node_count(sbi, dn->inode);
	set_node_addr(sbi, &ni, NULL_ADDR, false);

	if (dn->nid == dn->inode->i_ino) {
		remove_orphan_inode(sbi, dn->nid);
		dec_valid_inode_count(sbi);
		f2fs_inode_synced(dn->inode);
	}
invalidate:
	clear_node_page_dirty(dn->node_page);
	set_sbi_flag(sbi, SBI_IS_DIRTY);

	f2fs_put_page(dn->node_page, 1);

	invalidate_mapping_pages(NODE_MAPPING(sbi),
			dn->node_page->index, dn->node_page->index);

	dn->node_page = NULL;
	trace_f2fs_truncate_node(dn->inode, dn->nid, ni.blk_addr);
}
