static void __release_extent_node(struct f2fs_sb_info *sbi,
			struct extent_tree *et, struct extent_node *en)
{
	spin_lock(&sbi->extent_lock);
	f2fs_bug_on(sbi, list_empty(&en->list));
	list_del_init(&en->list);
	spin_unlock(&sbi->extent_lock);

	__detach_extent_node(sbi, et, en);
}
