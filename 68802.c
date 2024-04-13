static void update_free_nid_bitmap(struct f2fs_sb_info *sbi, nid_t nid,
							bool set, bool build)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);
	unsigned int nat_ofs = NAT_BLOCK_OFFSET(nid);
	unsigned int nid_ofs = nid - START_NID(nid);

	if (!test_bit_le(nat_ofs, nm_i->nat_block_bitmap))
		return;

	if (set)
		__set_bit_le(nid_ofs, nm_i->free_nid_bitmap[nat_ofs]);
	else
		__clear_bit_le(nid_ofs, nm_i->free_nid_bitmap[nat_ofs]);

	if (set)
		nm_i->free_nid_count[nat_ofs]++;
	else if (!build)
		nm_i->free_nid_count[nat_ofs]--;
}
