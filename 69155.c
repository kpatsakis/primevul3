static struct extent_node *__attach_extent_node(struct f2fs_sb_info *sbi,
				struct extent_tree *et, struct extent_info *ei,
				struct rb_node *parent, struct rb_node **p)
{
	struct extent_node *en;

	en = kmem_cache_alloc(extent_node_slab, GFP_ATOMIC);
	if (!en)
		return NULL;

	en->ei = *ei;
	INIT_LIST_HEAD(&en->list);
	en->et = et;

	rb_link_node(&en->rb_node, parent, p);
	rb_insert_color(&en->rb_node, &et->root);
	atomic_inc(&et->node_cnt);
	atomic_inc(&sbi->total_ext_node);
	return en;
}
