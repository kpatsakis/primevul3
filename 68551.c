do_add_counters(struct net *net, const void __user *user,
		unsigned int len, int compat)
{
	unsigned int i;
	struct xt_counters_info tmp;
	struct xt_counters *paddc;
	struct xt_table *t;
	const struct xt_table_info *private;
	int ret = 0;
	struct ipt_entry *iter;
	unsigned int addend;

	paddc = xt_copy_counters_from_user(user, len, &tmp, compat);
	if (IS_ERR(paddc))
		return PTR_ERR(paddc);

	t = xt_find_table_lock(net, AF_INET, tmp.name);
	if (IS_ERR(t)) {
		ret = PTR_ERR(t);
		goto free;
	}

	local_bh_disable();
	private = t->private;
	if (private->number != tmp.num_counters) {
		ret = -EINVAL;
		goto unlock_up_free;
	}

	i = 0;
	addend = xt_write_recseq_begin();
	xt_entry_foreach(iter, private->entries, private->size) {
		struct xt_counters *tmp;

		tmp = xt_get_this_cpu_counter(&iter->counters);
		ADD_COUNTER(*tmp, paddc[i].bcnt, paddc[i].pcnt);
		++i;
	}
	xt_write_recseq_end(addend);
 unlock_up_free:
	local_bh_enable();
	xt_table_unlock(t);
	module_put(t->me);
 free:
	vfree(paddc);

	return ret;
}
