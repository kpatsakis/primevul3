unsigned int f2fs_shrink_extent_tree(struct f2fs_sb_info *sbi, int nr_shrink)
{
	struct extent_tree *et, *next;
	struct extent_node *en;
	unsigned int node_cnt = 0, tree_cnt = 0;
	int remained;

	if (!test_opt(sbi, EXTENT_CACHE))
		return 0;

	if (!atomic_read(&sbi->total_zombie_tree))
		goto free_node;

	if (!mutex_trylock(&sbi->extent_tree_lock))
		goto out;

	/* 1. remove unreferenced extent tree */
	list_for_each_entry_safe(et, next, &sbi->zombie_list, list) {
		if (atomic_read(&et->node_cnt)) {
			write_lock(&et->lock);
			node_cnt += __free_extent_tree(sbi, et);
			write_unlock(&et->lock);
		}
		f2fs_bug_on(sbi, atomic_read(&et->node_cnt));
		list_del_init(&et->list);
		radix_tree_delete(&sbi->extent_tree_root, et->ino);
		kmem_cache_free(extent_tree_slab, et);
		atomic_dec(&sbi->total_ext_tree);
		atomic_dec(&sbi->total_zombie_tree);
		tree_cnt++;

		if (node_cnt + tree_cnt >= nr_shrink)
			goto unlock_out;
		cond_resched();
	}
	mutex_unlock(&sbi->extent_tree_lock);

free_node:
	/* 2. remove LRU extent entries */
	if (!mutex_trylock(&sbi->extent_tree_lock))
		goto out;

	remained = nr_shrink - (node_cnt + tree_cnt);

	spin_lock(&sbi->extent_lock);
	for (; remained > 0; remained--) {
		if (list_empty(&sbi->extent_list))
			break;
		en = list_first_entry(&sbi->extent_list,
					struct extent_node, list);
		et = en->et;
		if (!write_trylock(&et->lock)) {
			/* refresh this extent node's position in extent list */
			list_move_tail(&en->list, &sbi->extent_list);
			continue;
		}

		list_del_init(&en->list);
		spin_unlock(&sbi->extent_lock);

		__detach_extent_node(sbi, et, en);

		write_unlock(&et->lock);
		node_cnt++;
		spin_lock(&sbi->extent_lock);
	}
	spin_unlock(&sbi->extent_lock);

unlock_out:
	mutex_unlock(&sbi->extent_tree_lock);
out:
	trace_f2fs_shrink_extent_tree(sbi, node_cnt, tree_cnt);

	return node_cnt + tree_cnt;
}
