struct rb_node **__lookup_rb_tree_for_insert(struct f2fs_sb_info *sbi,
				struct rb_root *root, struct rb_node **parent,
				unsigned int ofs)
{
	struct rb_node **p = &root->rb_node;
	struct rb_entry *re;

	while (*p) {
		*parent = *p;
		re = rb_entry(*parent, struct rb_entry, rb_node);

		if (ofs < re->ofs)
			p = &(*p)->rb_left;
		else if (ofs >= re->ofs + re->len)
			p = &(*p)->rb_right;
		else
			f2fs_bug_on(sbi, 1);
	}

	return p;
}
