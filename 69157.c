static void __detach_extent_node(struct f2fs_sb_info *sbi,
				struct extent_tree *et, struct extent_node *en)
{
	rb_erase(&en->rb_node, &et->root);
	atomic_dec(&et->node_cnt);
	atomic_dec(&sbi->total_ext_node);

	if (et->cached_en == en)
		et->cached_en = NULL;
	kmem_cache_free(extent_node_slab, en);
}
