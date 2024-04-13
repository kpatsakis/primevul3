compat_get_entries(struct net *net, struct compat_ip6t_get_entries __user *uptr,
		   int *len)
{
	int ret;
	struct compat_ip6t_get_entries get;
	struct xt_table *t;

	if (*len < sizeof(get))
		return -EINVAL;

	if (copy_from_user(&get, uptr, sizeof(get)) != 0)
		return -EFAULT;

	if (*len != sizeof(struct compat_ip6t_get_entries) + get.size)
		return -EINVAL;

	get.name[sizeof(get.name) - 1] = '\0';

	xt_compat_lock(AF_INET6);
	t = xt_find_table_lock(net, AF_INET6, get.name);
	if (!IS_ERR(t)) {
		const struct xt_table_info *private = t->private;
		struct xt_table_info info;
		ret = compat_table_info(private, &info);
		if (!ret && get.size == info.size)
			ret = compat_copy_entries_to_user(private->size,
							  t, uptr->entrytable);
		else if (!ret)
			ret = -EAGAIN;

		xt_compat_flush_offsets(AF_INET6);
		module_put(t->me);
		xt_table_unlock(t);
	} else
		ret = PTR_ERR(t);

	xt_compat_unlock(AF_INET6);
	return ret;
}
