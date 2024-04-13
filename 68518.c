static int __do_replace(struct net *net, const char *name,
			unsigned int valid_hooks,
			struct xt_table_info *newinfo,
			unsigned int num_counters,
			void __user *counters_ptr)
{
	int ret;
	struct xt_table *t;
	struct xt_table_info *oldinfo;
	struct xt_counters *counters;
	void *loc_cpu_old_entry;
	struct arpt_entry *iter;

	ret = 0;
	counters = vzalloc(num_counters * sizeof(struct xt_counters));
	if (!counters) {
		ret = -ENOMEM;
		goto out;
	}

	t = xt_request_find_table_lock(net, NFPROTO_ARP, name);
	if (IS_ERR(t)) {
		ret = PTR_ERR(t);
		goto free_newinfo_counters_untrans;
	}

	/* You lied! */
	if (valid_hooks != t->valid_hooks) {
		ret = -EINVAL;
		goto put_module;
	}

	oldinfo = xt_replace_table(t, num_counters, newinfo, &ret);
	if (!oldinfo)
		goto put_module;

	/* Update module usage count based on number of rules */
	if ((oldinfo->number > oldinfo->initial_entries) ||
	    (newinfo->number <= oldinfo->initial_entries))
		module_put(t->me);
	if ((oldinfo->number > oldinfo->initial_entries) &&
	    (newinfo->number <= oldinfo->initial_entries))
		module_put(t->me);

	get_old_counters(oldinfo, counters);

	/* Decrease module usage counts and free resource */
	loc_cpu_old_entry = oldinfo->entries;
	xt_entry_foreach(iter, loc_cpu_old_entry, oldinfo->size)
		cleanup_entry(iter);

	xt_free_table_info(oldinfo);
	if (copy_to_user(counters_ptr, counters,
			 sizeof(struct xt_counters) * num_counters) != 0) {
		/* Silent error, can't fail, new table is already in place */
		net_warn_ratelimited("arptables: counters copy to user failed while replacing table\n");
	}
	vfree(counters);
	xt_table_unlock(t);
	return ret;

 put_module:
	module_put(t->me);
	xt_table_unlock(t);
 free_newinfo_counters_untrans:
	vfree(counters);
 out:
	return ret;
}
