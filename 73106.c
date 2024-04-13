u8 __dev_xdp_attached(struct net_device *dev, xdp_op_t xdp_op, u32 *prog_id)
{
	struct netdev_xdp xdp;

	memset(&xdp, 0, sizeof(xdp));
	xdp.command = XDP_QUERY_PROG;

	/* Query must always succeed. */
	WARN_ON(xdp_op(dev, &xdp) < 0);
	if (prog_id)
		*prog_id = xdp.prog_id;

	return xdp.prog_attached;
}
