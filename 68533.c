find_check_entry(struct arpt_entry *e, const char *name, unsigned int size,
		 struct xt_percpu_counter_alloc_state *alloc_state)
{
	struct xt_entry_target *t;
	struct xt_target *target;
	int ret;

	if (!xt_percpu_counter_alloc(alloc_state, &e->counters))
		return -ENOMEM;

	t = arpt_get_target(e);
	target = xt_request_find_target(NFPROTO_ARP, t->u.user.name,
					t->u.user.revision);
	if (IS_ERR(target)) {
		ret = PTR_ERR(target);
		goto out;
	}
	t->u.kernel.target = target;

	ret = check_target(e, name);
	if (ret)
		goto err;
	return 0;
err:
	module_put(t->u.kernel.target->me);
out:
	xt_percpu_counter_free(&e->counters);

	return ret;
}
