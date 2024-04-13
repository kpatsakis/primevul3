int register_pernet_device(struct pernet_operations *ops)
{
	int error;
	down_write(&pernet_ops_rwsem);
	error = register_pernet_operations(&pernet_list, ops);
	if (!error && (first_device == &pernet_list))
		first_device = &ops->list;
	up_write(&pernet_ops_rwsem);
	return error;
}
