static ssize_t tun_do_read(struct tun_struct *tun, struct tun_file *tfile,
			   struct iov_iter *to,
			   int noblock, struct sk_buff *skb)
{
	ssize_t ret;
	int err;

	tun_debug(KERN_INFO, tun, "tun_do_read\n");

	if (!iov_iter_count(to))
		return 0;

	if (!skb) {
		/* Read frames from ring */
		skb = tun_ring_recv(tfile, noblock, &err);
		if (!skb)
			return err;
	}

	ret = tun_put_user(tun, tfile, skb, to);
	if (unlikely(ret < 0))
		kfree_skb(skb);
	else
		consume_skb(skb);

	return ret;
}
