get_entries(struct net *net, struct ip6t_get_entries __user *uptr,
	    const int *len)
{
	int ret;
	struct ip6t_get_entries get;
	struct xt_table *t;

	if (*len < sizeof(get))
		return -EINVAL;
	if (copy_from_user(&get, uptr, sizeof(get)) != 0)
		return -EFAULT;
	if (*len != sizeof(struct ip6t_get_entries) + get.size)
		return -EINVAL;

	get.name[sizeof(get.name) - 1] = '\0';

	t = xt_find_table_lock(net, AF_INET6, get.name);
	if (!IS_ERR(t)) {
		struct xt_table_info *private = t->private;
		if (get.size == private->size)
			ret = copy_entries_to_user(private->size,
						   t, uptr->entrytable);
		else
			ret = -EAGAIN;

		module_put(t->me);
		xt_table_unlock(t);
	} else
		ret = PTR_ERR(t);

	return ret;
}
