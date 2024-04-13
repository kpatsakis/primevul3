void allocate_data_block(struct f2fs_sb_info *sbi, struct page *page,
		block_t old_blkaddr, block_t *new_blkaddr,
		struct f2fs_summary *sum, int type,
		struct f2fs_io_info *fio, bool add_list)
{
	struct sit_info *sit_i = SIT_I(sbi);
	struct curseg_info *curseg = CURSEG_I(sbi, type);

	mutex_lock(&curseg->curseg_mutex);
	mutex_lock(&sit_i->sentry_lock);

	*new_blkaddr = NEXT_FREE_BLKADDR(sbi, curseg);

	f2fs_wait_discard_bio(sbi, *new_blkaddr);

	/*
	 * __add_sum_entry should be resided under the curseg_mutex
	 * because, this function updates a summary entry in the
	 * current summary block.
	 */
	__add_sum_entry(sbi, type, sum);

	__refresh_next_blkoff(sbi, curseg);

	stat_inc_block_count(sbi, curseg);

	if (!__has_curseg_space(sbi, type))
		sit_i->s_ops->allocate_segment(sbi, type, false);
	/*
	 * SIT information should be updated after segment allocation,
	 * since we need to keep dirty segments precisely under SSR.
	 */
	refresh_sit_entry(sbi, old_blkaddr, *new_blkaddr);

	mutex_unlock(&sit_i->sentry_lock);

	if (page && IS_NODESEG(type)) {
		fill_node_footer_blkaddr(page, NEXT_FREE_BLKADDR(sbi, curseg));

		f2fs_inode_chksum_set(sbi, page);
	}

	if (add_list) {
		struct f2fs_bio_info *io;

		INIT_LIST_HEAD(&fio->list);
		fio->in_list = true;
		io = sbi->write_io[fio->type] + fio->temp;
		spin_lock(&io->io_lock);
		list_add_tail(&fio->list, &io->io_list);
		spin_unlock(&io->io_lock);
	}

	mutex_unlock(&curseg->curseg_mutex);
}
