static struct extent_node *__try_merge_extent_node(struct inode *inode,
				struct extent_tree *et, struct extent_info *ei,
				struct extent_node *prev_ex,
				struct extent_node *next_ex)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct extent_node *en = NULL;

	if (prev_ex && __is_back_mergeable(ei, &prev_ex->ei)) {
		prev_ex->ei.len += ei->len;
		ei = &prev_ex->ei;
		en = prev_ex;
	}

	if (next_ex && __is_front_mergeable(ei, &next_ex->ei)) {
		next_ex->ei.fofs = ei->fofs;
		next_ex->ei.blk = ei->blk;
		next_ex->ei.len += ei->len;
		if (en)
			__release_extent_node(sbi, et, prev_ex);

		en = next_ex;
	}

	if (!en)
		return NULL;

	__try_update_largest_extent(inode, et, en);

	spin_lock(&sbi->extent_lock);
	if (!list_empty(&en->list)) {
		list_move_tail(&en->list, &sbi->extent_list);
		et->cached_en = en;
	}
	spin_unlock(&sbi->extent_lock);
	return en;
}
