static struct extent_node *__insert_extent_tree(struct inode *inode,
				struct extent_tree *et, struct extent_info *ei,
				struct rb_node **insert_p,
				struct rb_node *insert_parent)
{
	struct f2fs_sb_info *sbi = F2FS_I_SB(inode);
	struct rb_node **p = &et->root.rb_node;
	struct rb_node *parent = NULL;
	struct extent_node *en = NULL;

	if (insert_p && insert_parent) {
		parent = insert_parent;
		p = insert_p;
		goto do_insert;
	}

	p = __lookup_rb_tree_for_insert(sbi, &et->root, &parent, ei->fofs);
do_insert:
	en = __attach_extent_node(sbi, et, ei, parent, p);
	if (!en)
		return NULL;

	__try_update_largest_extent(inode, et, en);

	/* update in global extent list */
	spin_lock(&sbi->extent_lock);
	list_add_tail(&en->list, &sbi->extent_list);
	et->cached_en = en;
	spin_unlock(&sbi->extent_lock);
	return en;
}
