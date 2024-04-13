static int __init genl_init(void)
{
	int err;

	err = genl_register_family(&genl_ctrl);
	if (err < 0)
		goto problem;

	err = register_pernet_subsys(&genl_pernet_ops);
	if (err)
		goto problem;

	return 0;

problem:
	panic("GENL: Cannot register controller: %d\n", err);
}
