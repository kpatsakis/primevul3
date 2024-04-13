int dev_change_xdp_fd(struct net_device *dev, struct netlink_ext_ack *extack,
		      int fd, u32 flags)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	struct bpf_prog *prog = NULL;
	xdp_op_t xdp_op, xdp_chk;
	int err;

	ASSERT_RTNL();

	xdp_op = xdp_chk = ops->ndo_xdp;
	if (!xdp_op && (flags & (XDP_FLAGS_DRV_MODE | XDP_FLAGS_HW_MODE)))
		return -EOPNOTSUPP;
	if (!xdp_op || (flags & XDP_FLAGS_SKB_MODE))
		xdp_op = generic_xdp_install;
	if (xdp_op == xdp_chk)
		xdp_chk = generic_xdp_install;

	if (fd >= 0) {
		if (xdp_chk && __dev_xdp_attached(dev, xdp_chk, NULL))
			return -EEXIST;
		if ((flags & XDP_FLAGS_UPDATE_IF_NOEXIST) &&
		    __dev_xdp_attached(dev, xdp_op, NULL))
			return -EBUSY;

		prog = bpf_prog_get_type(fd, BPF_PROG_TYPE_XDP);
		if (IS_ERR(prog))
			return PTR_ERR(prog);
	}

	err = dev_xdp_install(dev, xdp_op, extack, flags, prog);
	if (err < 0 && prog)
		bpf_prog_put(prog);

	return err;
}
