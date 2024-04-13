bool __check_rb_tree_consistence(struct f2fs_sb_info *sbi,
						struct rb_root *root)
{
#ifdef CONFIG_F2FS_CHECK_FS
	struct rb_node *cur = rb_first(root), *next;
	struct rb_entry *cur_re, *next_re;

	if (!cur)
		return true;

	while (cur) {
		next = rb_next(cur);
		if (!next)
			return true;

		cur_re = rb_entry(cur, struct rb_entry, rb_node);
		next_re = rb_entry(next, struct rb_entry, rb_node);

		if (cur_re->ofs + cur_re->len > next_re->ofs) {
			f2fs_msg(sbi->sb, KERN_INFO, "inconsistent rbtree, "
				"cur(%u, %u) next(%u, %u)",
				cur_re->ofs, cur_re->len,
				next_re->ofs, next_re->len);
			return false;
		}

		cur = next;
	}
#endif
	return true;
}
