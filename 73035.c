static int tun_chr_open(struct inode *inode, struct file * file)
{
	struct net *net = current->nsproxy->net_ns;
	struct tun_file *tfile;

	DBG1(KERN_INFO, "tunX: tun_chr_open\n");

	tfile = (struct tun_file *)sk_alloc(net, AF_UNSPEC, GFP_KERNEL,
					    &tun_proto, 0);
	if (!tfile)
		return -ENOMEM;
	RCU_INIT_POINTER(tfile->tun, NULL);
	tfile->flags = 0;
	tfile->ifindex = 0;

	init_waitqueue_head(&tfile->wq.wait);
	RCU_INIT_POINTER(tfile->socket.wq, &tfile->wq);

	tfile->socket.file = file;
	tfile->socket.ops = &tun_socket_ops;

	sock_init_data(&tfile->socket, &tfile->sk);

	tfile->sk.sk_write_space = tun_sock_write_space;
	tfile->sk.sk_sndbuf = INT_MAX;

	file->private_data = tfile;
	INIT_LIST_HEAD(&tfile->next);

	sock_set_flag(&tfile->sk, SOCK_ZEROCOPY);

	return 0;
}
