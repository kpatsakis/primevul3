do_ipt_get_ctl(struct sock *sk, int cmd, void __user *user, int *len)
{
	int ret;

	if (!ns_capable(sock_net(sk)->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	switch (cmd) {
	case IPT_SO_GET_INFO:
		ret = get_info(sock_net(sk), user, len, 0);
		break;

	case IPT_SO_GET_ENTRIES:
		ret = get_entries(sock_net(sk), user, len);
		break;

	case IPT_SO_GET_REVISION_MATCH:
	case IPT_SO_GET_REVISION_TARGET: {
		struct xt_get_revision rev;
		int target;

		if (*len != sizeof(rev)) {
			ret = -EINVAL;
			break;
		}
		if (copy_from_user(&rev, user, sizeof(rev)) != 0) {
			ret = -EFAULT;
			break;
		}
		rev.name[sizeof(rev.name)-1] = 0;

		if (cmd == IPT_SO_GET_REVISION_TARGET)
			target = 1;
		else
			target = 0;

		try_then_request_module(xt_find_revision(AF_INET, rev.name,
							 rev.revision,
							 target, &ret),
					"ipt_%s", rev.name);
		break;
	}

	default:
		ret = -EINVAL;
	}

	return ret;
}
