void unregister_pernet_subsys(struct pernet_operations *ops)
{
	down_write(&pernet_ops_rwsem);
	unregister_pernet_operations(ops);
	up_write(&pernet_ops_rwsem);
}
