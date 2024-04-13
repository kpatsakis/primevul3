static const struct genl_ops *genl_get_cmd(u8 cmd,
					   const struct genl_family *family)
{
	int i;

	for (i = 0; i < family->n_ops; i++)
		if (family->ops[i].cmd == cmd)
			return &family->ops[i];

	return NULL;
}
