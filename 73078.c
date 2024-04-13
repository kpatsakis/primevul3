static int tun_peek_len(struct socket *sock)
{
	struct tun_file *tfile = container_of(sock, struct tun_file, socket);
	struct tun_struct *tun;
	int ret = 0;

	tun = __tun_get(tfile);
	if (!tun)
		return 0;

	ret = skb_array_peek_len(&tfile->tx_array);
	tun_put(tun);

	return ret;
}
