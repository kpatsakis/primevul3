static void scan_nat_page(struct f2fs_sb_info *sbi,
			struct page *nat_page, nid_t start_nid)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	struct f2fs_nat_block *nat_blk = page_address(nat_page);
	block_t blk_addr;
	unsigned int nat_ofs = NAT_BLOCK_OFFSET(start_nid);
	int i;

	if (test_bit_le(nat_ofs, nm_i->nat_block_bitmap))
		return;

	__set_bit_le(nat_ofs, nm_i->nat_block_bitmap);

	i = start_nid % NAT_ENTRY_PER_BLOCK;

	for (; i < NAT_ENTRY_PER_BLOCK; i++, start_nid++) {
		bool freed = false;

		if (unlikely(start_nid >= nm_i->max_nid))
			break;

		blk_addr = le32_to_cpu(nat_blk->entries[i].block_addr);
		f2fs_bug_on(sbi, blk_addr == NEW_ADDR);
		if (blk_addr == NULL_ADDR)
			freed = add_free_nid(sbi, start_nid, true);
		spin_lock(&NM_I(sbi)->nid_list_lock);
		update_free_nid_bitmap(sbi, start_nid, freed, true);
		spin_unlock(&NM_I(sbi)->nid_list_lock);
	}
}
