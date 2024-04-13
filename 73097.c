static u32 tun_xdp_query(struct net_device *dev)
{
	struct tun_struct *tun = netdev_priv(dev);
	const struct bpf_prog *xdp_prog;

	xdp_prog = rtnl_dereference(tun->xdp_prog);
	if (xdp_prog)
		return xdp_prog->aux->id;

	return 0;
}
