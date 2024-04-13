static void unregister_pernet_operations(struct pernet_operations *ops)
{
	__unregister_pernet_operations(ops);
	rcu_barrier();
	if (ops->id)
		ida_free(&net_generic_ids, *ops->id);
}
