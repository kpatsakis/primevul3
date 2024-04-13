static int genl_validate_ops(const struct genl_family *family)
{
	const struct genl_ops *ops = family->ops;
	unsigned int n_ops = family->n_ops;
	int i, j;

	if (WARN_ON(n_ops && !ops))
		return -EINVAL;

	if (!n_ops)
		return 0;

	for (i = 0; i < n_ops; i++) {
		if (ops[i].dumpit == NULL && ops[i].doit == NULL)
			return -EINVAL;
		for (j = i + 1; j < n_ops; j++)
			if (ops[i].cmd == ops[j].cmd)
				return -EINVAL;
	}

	return 0;
}
