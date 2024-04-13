static int ipip6_tunnel_get_prl(struct ip_tunnel *t,
				struct ip_tunnel_prl __user *a)
{
	struct ip_tunnel_prl kprl, *kp;
	struct ip_tunnel_prl_entry *prl;
	unsigned int cmax, c = 0, ca, len;
	int ret = 0;

	if (copy_from_user(&kprl, a, sizeof(kprl)))
		return -EFAULT;
	cmax = kprl.datalen / sizeof(kprl);
	if (cmax > 1 && kprl.addr != htonl(INADDR_ANY))
		cmax = 1;

	/* For simple GET or for root users,
	 * we try harder to allocate.
	 */
	kp = (cmax <= 1 || capable(CAP_NET_ADMIN)) ?
		kcalloc(cmax, sizeof(*kp), GFP_KERNEL | __GFP_NOWARN) :
		NULL;

	rcu_read_lock();

	ca = t->prl_count < cmax ? t->prl_count : cmax;

	if (!kp) {
		/* We don't try hard to allocate much memory for
		 * non-root users.
		 * For root users, retry allocating enough memory for
		 * the answer.
		 */
		kp = kcalloc(ca, sizeof(*kp), GFP_ATOMIC);
		if (!kp) {
			ret = -ENOMEM;
			goto out;
		}
	}

	c = 0;
	for_each_prl_rcu(t->prl) {
		if (c >= cmax)
			break;
		if (kprl.addr != htonl(INADDR_ANY) && prl->addr != kprl.addr)
			continue;
		kp[c].addr = prl->addr;
		kp[c].flags = prl->flags;
		c++;
		if (kprl.addr != htonl(INADDR_ANY))
			break;
	}
out:
	rcu_read_unlock();

	len = sizeof(*kp) * c;
	ret = 0;
	if ((len && copy_to_user(a + 1, kp, len)) || put_user(len, &a->datalen))
		ret = -EFAULT;

	kfree(kp);

	return ret;
}
