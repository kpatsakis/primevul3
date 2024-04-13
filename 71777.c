int register_pernet_subsys(struct pernet_operations *ops)
{
	int error;
	down_write(&pernet_ops_rwsem);
	error =  register_pernet_operations(first_device, ops);
	up_write(&pernet_ops_rwsem);
	return error;
}
