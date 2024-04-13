static int generic_xdp_install(struct net_device *dev, struct netdev_xdp *xdp)
{
	struct bpf_prog *old = rtnl_dereference(dev->xdp_prog);
	struct bpf_prog *new = xdp->prog;
	int ret = 0;

	switch (xdp->command) {
	case XDP_SETUP_PROG:
		rcu_assign_pointer(dev->xdp_prog, new);
		if (old)
			bpf_prog_put(old);

		if (old && !new) {
			static_key_slow_dec(&generic_xdp_needed);
		} else if (new && !old) {
			static_key_slow_inc(&generic_xdp_needed);
			dev_disable_lro(dev);
		}
		break;

	case XDP_QUERY_PROG:
		xdp->prog_attached = !!old;
		xdp->prog_id = old ? old->aux->id : 0;
		break;

	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}
