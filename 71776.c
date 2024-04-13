static int register_pernet_operations(struct list_head *list,
				      struct pernet_operations *ops)
{
	int error;

	if (ops->id) {
		error = ida_alloc_min(&net_generic_ids, MIN_PERNET_OPS_ID,
				GFP_KERNEL);
		if (error < 0)
			return error;
		*ops->id = error;
		max_gen_ptrs = max(max_gen_ptrs, *ops->id + 1);
	}
	error = __register_pernet_operations(list, ops);
	if (error) {
		rcu_barrier();
		if (ops->id)
			ida_free(&net_generic_ids, *ops->id);
	}

	return error;
}
