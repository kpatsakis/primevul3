static int init_free_nid_cache(struct f2fs_sb_info *sbi)
{
	struct f2fs_nm_info *nm_i = NM_I(sbi);

	nm_i->free_nid_bitmap = f2fs_kvzalloc(nm_i->nat_blocks *
					NAT_ENTRY_BITMAP_SIZE, GFP_KERNEL);
	if (!nm_i->free_nid_bitmap)
		return -ENOMEM;

	nm_i->nat_block_bitmap = f2fs_kvzalloc(nm_i->nat_blocks / 8,
								GFP_KERNEL);
	if (!nm_i->nat_block_bitmap)
		return -ENOMEM;

	nm_i->free_nid_count = f2fs_kvzalloc(nm_i->nat_blocks *
					sizeof(unsigned short), GFP_KERNEL);
	if (!nm_i->free_nid_count)
		return -ENOMEM;
	return 0;
}
