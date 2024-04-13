int arpt_register_table(struct net *net,
			const struct xt_table *table,
			const struct arpt_replace *repl,
			const struct nf_hook_ops *ops,
			struct xt_table **res)
{
	int ret;
	struct xt_table_info *newinfo;
	struct xt_table_info bootstrap = {0};
	void *loc_cpu_entry;
	struct xt_table *new_table;

	newinfo = xt_alloc_table_info(repl->size);
	if (!newinfo)
		return -ENOMEM;

	loc_cpu_entry = newinfo->entries;
	memcpy(loc_cpu_entry, repl->entries, repl->size);

	ret = translate_table(newinfo, loc_cpu_entry, repl);
	if (ret != 0)
		goto out_free;

	new_table = xt_register_table(net, table, &bootstrap, newinfo);
	if (IS_ERR(new_table)) {
		ret = PTR_ERR(new_table);
		goto out_free;
	}

	/* set res now, will see skbs right after nf_register_net_hooks */
	WRITE_ONCE(*res, new_table);

	ret = nf_register_net_hooks(net, ops, hweight32(table->valid_hooks));
	if (ret != 0) {
		__arpt_unregister_table(new_table);
		*res = NULL;
	}

	return ret;

out_free:
	xt_free_table_info(newinfo);
	return ret;
}
