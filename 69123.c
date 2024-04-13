static void update_sit_entry(struct f2fs_sb_info *sbi, block_t blkaddr, int del)
{
	struct seg_entry *se;
	unsigned int segno, offset;
	long int new_vblocks;
	bool exist;
#ifdef CONFIG_F2FS_CHECK_FS
	bool mir_exist;
#endif

	segno = GET_SEGNO(sbi, blkaddr);

	se = get_seg_entry(sbi, segno);
	new_vblocks = se->valid_blocks + del;
	offset = GET_BLKOFF_FROM_SEG0(sbi, blkaddr);

	f2fs_bug_on(sbi, (new_vblocks >> (sizeof(unsigned short) << 3) ||
				(new_vblocks > sbi->blocks_per_seg)));

	se->valid_blocks = new_vblocks;
	se->mtime = get_mtime(sbi);
	SIT_I(sbi)->max_mtime = se->mtime;

	/* Update valid block bitmap */
	if (del > 0) {
		exist = f2fs_test_and_set_bit(offset, se->cur_valid_map);
#ifdef CONFIG_F2FS_CHECK_FS
		mir_exist = f2fs_test_and_set_bit(offset,
						se->cur_valid_map_mir);
		if (unlikely(exist != mir_exist)) {
			f2fs_msg(sbi->sb, KERN_ERR, "Inconsistent error "
				"when setting bitmap, blk:%u, old bit:%d",
				blkaddr, exist);
			f2fs_bug_on(sbi, 1);
		}
#endif
		if (unlikely(exist)) {
			f2fs_msg(sbi->sb, KERN_ERR,
				"Bitmap was wrongly set, blk:%u", blkaddr);
			f2fs_bug_on(sbi, 1);
			se->valid_blocks--;
			del = 0;
		}

		if (f2fs_discard_en(sbi) &&
			!f2fs_test_and_set_bit(offset, se->discard_map))
			sbi->discard_blks--;

		/* don't overwrite by SSR to keep node chain */
		if (se->type == CURSEG_WARM_NODE) {
			if (!f2fs_test_and_set_bit(offset, se->ckpt_valid_map))
				se->ckpt_valid_blocks++;
		}
	} else {
		exist = f2fs_test_and_clear_bit(offset, se->cur_valid_map);
#ifdef CONFIG_F2FS_CHECK_FS
		mir_exist = f2fs_test_and_clear_bit(offset,
						se->cur_valid_map_mir);
		if (unlikely(exist != mir_exist)) {
			f2fs_msg(sbi->sb, KERN_ERR, "Inconsistent error "
				"when clearing bitmap, blk:%u, old bit:%d",
				blkaddr, exist);
			f2fs_bug_on(sbi, 1);
		}
#endif
		if (unlikely(!exist)) {
			f2fs_msg(sbi->sb, KERN_ERR,
				"Bitmap was wrongly cleared, blk:%u", blkaddr);
			f2fs_bug_on(sbi, 1);
			se->valid_blocks++;
			del = 0;
		}

		if (f2fs_discard_en(sbi) &&
			f2fs_test_and_clear_bit(offset, se->discard_map))
			sbi->discard_blks++;
	}
	if (!f2fs_test_bit(offset, se->ckpt_valid_map))
		se->ckpt_valid_blocks += del;

	__mark_sit_entry_dirty(sbi, segno);

	/* update total number of valid blocks to be written in ckpt area */
	SIT_I(sbi)->written_valid_blocks += del;

	if (sbi->segs_per_sec > 1)
		get_sec_entry(sbi, segno)->valid_blocks += del;
}
