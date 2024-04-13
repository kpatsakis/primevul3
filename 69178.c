static void f2fs_update_extent_tree_range(struct inode *inode,
				pgoff_t fofs, block_t blkaddr, unsigned int len)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct extent_tree *et = F2FS_I(inode)->extent_tree;
	struct extent_node *en = NULL, *en1 = NULL;
	struct extent_node *prev_en = NULL, *next_en = NULL;
	struct extent_info ei, dei, prev;
	struct rb_node **insert_p = NULL, *insert_parent = NULL;
	unsigned int end = fofs + len;
	unsigned int pos = (unsigned int)fofs;

	if (!et)
		return;

	trace_f2fs_update_extent_tree_range(inode, fofs, blkaddr, len);

	write_lock(&et->lock);

	if (is_inode_flag_set(inode, FI_NO_EXTENT)) {
		write_unlock(&et->lock);
		return;
	}

	prev = et->largest;
	dei.len = 0;

	/*
	 * drop largest extent before lookup, in case it's already
	 * been shrunk from extent tree
	 */
	__drop_largest_extent(inode, fofs, len);

	/* 1. lookup first extent node in range [fofs, fofs + len - 1] */
	en = (struct extent_node *)__lookup_rb_tree_ret(&et->root,
					(struct rb_entry *)et->cached_en, fofs,
					(struct rb_entry **)&prev_en,
					(struct rb_entry **)&next_en,
					&insert_p, &insert_parent, false);
	if (!en)
		en = next_en;

	/* 2. invlidate all extent nodes in range [fofs, fofs + len - 1] */
	while (en && en->ei.fofs < end) {
		unsigned int org_end;
		int parts = 0;	/* # of parts current extent split into */

		next_en = en1 = NULL;

		dei = en->ei;
		org_end = dei.fofs + dei.len;
		f2fs_bug_on(sbi, pos >= org_end);

		if (pos > dei.fofs &&	pos - dei.fofs >= F2FS_MIN_EXTENT_LEN) {
			en->ei.len = pos - en->ei.fofs;
			prev_en = en;
			parts = 1;
		}

		if (end < org_end && org_end - end >= F2FS_MIN_EXTENT_LEN) {
			if (parts) {
				set_extent_info(&ei, end,
						end - dei.fofs + dei.blk,
						org_end - end);
				en1 = __insert_extent_tree(inode, et, &ei,
							NULL, NULL);
				next_en = en1;
			} else {
				en->ei.fofs = end;
				en->ei.blk += end - dei.fofs;
				en->ei.len -= end - dei.fofs;
				next_en = en;
			}
			parts++;
		}

		if (!next_en) {
			struct rb_node *node = rb_next(&en->rb_node);

			next_en = rb_entry_safe(node, struct extent_node,
						rb_node);
		}

		if (parts)
			__try_update_largest_extent(inode, et, en);
		else
			__release_extent_node(sbi, et, en);

		/*
		 * if original extent is split into zero or two parts, extent
		 * tree has been altered by deletion or insertion, therefore
		 * invalidate pointers regard to tree.
		 */
		if (parts != 1) {
			insert_p = NULL;
			insert_parent = NULL;
		}
		en = next_en;
	}

	/* 3. update extent in extent cache */
	if (blkaddr) {

		set_extent_info(&ei, fofs, blkaddr, len);
		if (!__try_merge_extent_node(inode, et, &ei, prev_en, next_en))
			__insert_extent_tree(inode, et, &ei,
						insert_p, insert_parent);

		/* give up extent_cache, if split and small updates happen */
		if (dei.len >= 1 &&
				prev.len < F2FS_MIN_EXTENT_LEN &&
				et->largest.len < F2FS_MIN_EXTENT_LEN) {
			__drop_largest_extent(inode, 0, UINT_MAX);
			set_inode_flag(inode, FI_NO_EXTENT);
		}
	}

	if (is_inode_flag_set(inode, FI_NO_EXTENT))
		__free_extent_tree(sbi, et);

	write_unlock(&et->lock);
}
