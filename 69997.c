ipip6_tunnel_del_prl(struct ip_tunnel *t, struct ip_tunnel_prl *a)
{
	struct ip_tunnel_prl_entry *x;
	struct ip_tunnel_prl_entry __rcu **p;
	int err = 0;

	ASSERT_RTNL();

	if (a && a->addr != htonl(INADDR_ANY)) {
		for (p = &t->prl;
		     (x = rtnl_dereference(*p)) != NULL;
		     p = &x->next) {
			if (x->addr == a->addr) {
				*p = x->next;
				kfree_rcu(x, rcu_head);
				t->prl_count--;
				goto out;
			}
		}
		err = -ENXIO;
	} else {
		x = rtnl_dereference(t->prl);
		if (x) {
			t->prl_count = 0;
			call_rcu(&x->rcu_head, prl_list_destroy_rcu);
			t->prl = NULL;
		}
	}
out:
	return err;
}
