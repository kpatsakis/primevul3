static int build_sit_info(struct f2fs_sb_info *sbi)
{
	struct f2fs_super_block *raw_super = F2FS_RAW_SUPER(sbi);
	struct sit_info *sit_i;
	unsigned int sit_segs, start;
	char *src_bitmap;
	unsigned int bitmap_size;

	/* allocate memory for SIT information */
	sit_i = kzalloc(sizeof(struct sit_info), GFP_KERNEL);
	if (!sit_i)
		return -ENOMEM;

	SM_I(sbi)->sit_info = sit_i;

	sit_i->sentries = kvzalloc(MAIN_SEGS(sbi) *
					sizeof(struct seg_entry), GFP_KERNEL);
	if (!sit_i->sentries)
		return -ENOMEM;

	bitmap_size = f2fs_bitmap_size(MAIN_SEGS(sbi));
	sit_i->dirty_sentries_bitmap = kvzalloc(bitmap_size, GFP_KERNEL);
	if (!sit_i->dirty_sentries_bitmap)
		return -ENOMEM;

	for (start = 0; start < MAIN_SEGS(sbi); start++) {
		sit_i->sentries[start].cur_valid_map
			= kzalloc(SIT_VBLOCK_MAP_SIZE, GFP_KERNEL);
		sit_i->sentries[start].ckpt_valid_map
			= kzalloc(SIT_VBLOCK_MAP_SIZE, GFP_KERNEL);
		if (!sit_i->sentries[start].cur_valid_map ||
				!sit_i->sentries[start].ckpt_valid_map)
			return -ENOMEM;

#ifdef CONFIG_F2FS_CHECK_FS
		sit_i->sentries[start].cur_valid_map_mir
			= kzalloc(SIT_VBLOCK_MAP_SIZE, GFP_KERNEL);
		if (!sit_i->sentries[start].cur_valid_map_mir)
			return -ENOMEM;
#endif

		if (f2fs_discard_en(sbi)) {
			sit_i->sentries[start].discard_map
				= kzalloc(SIT_VBLOCK_MAP_SIZE, GFP_KERNEL);
			if (!sit_i->sentries[start].discard_map)
				return -ENOMEM;
		}
	}

	sit_i->tmp_map = kzalloc(SIT_VBLOCK_MAP_SIZE, GFP_KERNEL);
	if (!sit_i->tmp_map)
		return -ENOMEM;

	if (sbi->segs_per_sec > 1) {
		sit_i->sec_entries = kvzalloc(MAIN_SECS(sbi) *
					sizeof(struct sec_entry), GFP_KERNEL);
		if (!sit_i->sec_entries)
			return -ENOMEM;
	}

	/* get information related with SIT */
	sit_segs = le32_to_cpu(raw_super->segment_count_sit) >> 1;

	/* setup SIT bitmap from ckeckpoint pack */
	bitmap_size = __bitmap_size(sbi, SIT_BITMAP);
	src_bitmap = __bitmap_ptr(sbi, SIT_BITMAP);

	sit_i->sit_bitmap = kmemdup(src_bitmap, bitmap_size, GFP_KERNEL);
	if (!sit_i->sit_bitmap)
		return -ENOMEM;

#ifdef CONFIG_F2FS_CHECK_FS
	sit_i->sit_bitmap_mir = kmemdup(src_bitmap, bitmap_size, GFP_KERNEL);
	if (!sit_i->sit_bitmap_mir)
		return -ENOMEM;
#endif

	/* init SIT information */
	sit_i->s_ops = &default_salloc_ops;

	sit_i->sit_base_addr = le32_to_cpu(raw_super->sit_blkaddr);
	sit_i->sit_blocks = sit_segs << sbi->log_blocks_per_seg;
	sit_i->written_valid_blocks = 0;
	sit_i->bitmap_size = bitmap_size;
	sit_i->dirty_sentries = 0;
	sit_i->sents_per_block = SIT_ENTRY_PER_BLOCK;
	sit_i->elapsed_time = le64_to_cpu(sbi->ckpt->elapsed_time);
	sit_i->mounted_time = ktime_get_real_seconds();
	mutex_init(&sit_i->sentry_lock);
	return 0;
}
