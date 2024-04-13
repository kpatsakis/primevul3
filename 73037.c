static void tun_chr_show_fdinfo(struct seq_file *m, struct file *f)
{
	struct tun_struct *tun;
	struct ifreq ifr;

	memset(&ifr, 0, sizeof(ifr));

	rtnl_lock();
	tun = tun_get(f);
	if (tun)
		tun_get_iff(current->nsproxy->net_ns, tun, &ifr);
	rtnl_unlock();

	if (tun)
		tun_put(tun);

	seq_printf(m, "iff:\t%s\n", ifr.ifr_name);
}
