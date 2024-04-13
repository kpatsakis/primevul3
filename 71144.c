static int hidp_sock_compat_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	void __user *argp = compat_ptr(arg);
	int err;

	if (cmd == HIDPGETCONNLIST) {
		struct hidp_connlist_req cl;
		u32 __user *p = argp;
		u32 uci;

		if (get_user(cl.cnum, p) || get_user(uci, p + 1))
			return -EFAULT;

		cl.ci = compat_ptr(uci);

		if (cl.cnum <= 0)
			return -EINVAL;

		err = hidp_get_connlist(&cl);

		if (!err && put_user(cl.cnum, p))
			err = -EFAULT;

		return err;
	} else if (cmd == HIDPCONNADD) {
		struct compat_hidp_connadd_req ca32;
		struct hidp_connadd_req ca;
		struct socket *csock;
		struct socket *isock;

		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if (copy_from_user(&ca32, (void __user *) arg, sizeof(ca32)))
			return -EFAULT;

		ca.ctrl_sock = ca32.ctrl_sock;
		ca.intr_sock = ca32.intr_sock;
		ca.parser = ca32.parser;
		ca.rd_size = ca32.rd_size;
		ca.rd_data = compat_ptr(ca32.rd_data);
		ca.country = ca32.country;
		ca.subclass = ca32.subclass;
		ca.vendor = ca32.vendor;
		ca.product = ca32.product;
		ca.version = ca32.version;
		ca.flags = ca32.flags;
		ca.idle_to = ca32.idle_to;
		memcpy(ca.name, ca32.name, 128);

		csock = sockfd_lookup(ca.ctrl_sock, &err);
		if (!csock)
			return err;

		isock = sockfd_lookup(ca.intr_sock, &err);
		if (!isock) {
			sockfd_put(csock);
			return err;
		}

		err = hidp_connection_add(&ca, csock, isock);
		if (!err && copy_to_user(argp, &ca32, sizeof(ca32)))
			err = -EFAULT;

		sockfd_put(csock);
		sockfd_put(isock);

		return err;
	}

	return hidp_sock_ioctl(sock, cmd, arg);
}
