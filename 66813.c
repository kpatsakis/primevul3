bool sock_is_registered(int family)
{
	return family < NPROTO && rcu_access_pointer(net_families[family]);
}
