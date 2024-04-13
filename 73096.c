static int tun_xdp(struct net_device *dev, struct netdev_xdp *xdp)
{
	switch (xdp->command) {
	case XDP_SETUP_PROG:
		return tun_xdp_set(dev, xdp->prog, xdp->extack);
	case XDP_QUERY_PROG:
		xdp->prog_id = tun_xdp_query(dev);
		xdp->prog_attached = !!xdp->prog_id;
		return 0;
	default:
		return -EINVAL;
	}
}
