static struct extent_node *__init_extent_tree(struct f2fs_sb_info *sbi,
				struct extent_tree *et, struct extent_info *ei)
{
	struct rb_node **p = &et->root.rb_node;
	struct extent_node *en;

	en = __attach_extent_node(sbi, et, ei, NULL, p);
	if (!en)
		return NULL;

	et->largest = en->ei;
	et->cached_en = en;
	return en;
}
