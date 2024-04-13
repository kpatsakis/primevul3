static long sock_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	struct socket *sock;
	struct sock *sk;
	void __user *argp = (void __user *)arg;
	int pid, err;
	struct net *net;

	sock = file->private_data;
	sk = sock->sk;
	net = sock_net(sk);
	if (unlikely(cmd >= SIOCDEVPRIVATE && cmd <= (SIOCDEVPRIVATE + 15))) {
		struct ifreq ifr;
		bool need_copyout;
		if (copy_from_user(&ifr, argp, sizeof(struct ifreq)))
			return -EFAULT;
		err = dev_ioctl(net, cmd, &ifr, &need_copyout);
		if (!err && need_copyout)
			if (copy_to_user(argp, &ifr, sizeof(struct ifreq)))
				return -EFAULT;
	} else
#ifdef CONFIG_WEXT_CORE
	if (cmd >= SIOCIWFIRST && cmd <= SIOCIWLAST) {
		err = wext_handle_ioctl(net, cmd, argp);
	} else
#endif
		switch (cmd) {
		case FIOSETOWN:
		case SIOCSPGRP:
			err = -EFAULT;
			if (get_user(pid, (int __user *)argp))
				break;
			err = f_setown(sock->file, pid, 1);
			break;
		case FIOGETOWN:
		case SIOCGPGRP:
			err = put_user(f_getown(sock->file),
				       (int __user *)argp);
			break;
		case SIOCGIFBR:
		case SIOCSIFBR:
		case SIOCBRADDBR:
		case SIOCBRDELBR:
			err = -ENOPKG;
			if (!br_ioctl_hook)
				request_module("bridge");

			mutex_lock(&br_ioctl_mutex);
			if (br_ioctl_hook)
				err = br_ioctl_hook(net, cmd, argp);
			mutex_unlock(&br_ioctl_mutex);
			break;
		case SIOCGIFVLAN:
		case SIOCSIFVLAN:
			err = -ENOPKG;
			if (!vlan_ioctl_hook)
				request_module("8021q");

			mutex_lock(&vlan_ioctl_mutex);
			if (vlan_ioctl_hook)
				err = vlan_ioctl_hook(net, argp);
			mutex_unlock(&vlan_ioctl_mutex);
			break;
		case SIOCADDDLCI:
		case SIOCDELDLCI:
			err = -ENOPKG;
			if (!dlci_ioctl_hook)
				request_module("dlci");

			mutex_lock(&dlci_ioctl_mutex);
			if (dlci_ioctl_hook)
				err = dlci_ioctl_hook(cmd, argp);
			mutex_unlock(&dlci_ioctl_mutex);
			break;
		case SIOCGSKNS:
			err = -EPERM;
			if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
				break;

			err = open_related_ns(&net->ns, get_net_ns);
			break;
		default:
			err = sock_do_ioctl(net, sock, cmd, arg);
			break;
		}
	return err;
}
