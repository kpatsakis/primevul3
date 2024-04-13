inline void load_free_nid_bitmap(struct f2fs_sb_info *sbi)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	unsigned int i = 0;
	nid_t nid, last_nid;

	if (!enabled_nat_bits(sbi, NULL))
		return;

	for (i = 0; i < nm_i->nat_blocks; i++) {
		i = find_next_bit_le(nm_i->empty_nat_bits, nm_i->nat_blocks, i);
		if (i >= nm_i->nat_blocks)
			break;

		__set_bit_le(i, nm_i->nat_block_bitmap);

		nid = i * NAT_ENTRY_PER_BLOCK;
		last_nid = (i + 1) * NAT_ENTRY_PER_BLOCK;

		spin_lock(&NM_I(sbi)->nid_list_lock);
		for (; nid < last_nid; nid++)
			update_free_nid_bitmap(sbi, nid, true, true);
		spin_unlock(&NM_I(sbi)->nid_list_lock);
	}

	for (i = 0; i < nm_i->nat_blocks; i++) {
		i = find_next_bit_le(nm_i->full_nat_bits, nm_i->nat_blocks, i);
		if (i >= nm_i->nat_blocks)
			break;

		__set_bit_le(i, nm_i->nat_block_bitmap);
	}
}
