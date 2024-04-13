static int tun_xdp_set(struct net_device *dev, struct bpf_prog *prog,
		       struct netlink_ext_ack *extack)
{
	struct tun_struct *tun = netdev_priv(dev);
	struct bpf_prog *old_prog;

	old_prog = rtnl_dereference(tun->xdp_prog);
	rcu_assign_pointer(tun->xdp_prog, prog);
	if (old_prog)
		bpf_prog_put(old_prog);

	return 0;
}
