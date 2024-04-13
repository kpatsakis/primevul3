static int tun_queue_resize(struct tun_struct *tun)
{
	struct net_device *dev = tun->dev;
	struct tun_file *tfile;
	struct skb_array **arrays;
	int n = tun->numqueues + tun->numdisabled;
	int ret, i;

	arrays = kmalloc_array(n, sizeof(*arrays), GFP_KERNEL);
	if (!arrays)
		return -ENOMEM;

	for (i = 0; i < tun->numqueues; i++) {
		tfile = rtnl_dereference(tun->tfiles[i]);
		arrays[i] = &tfile->tx_array;
	}
	list_for_each_entry(tfile, &tun->disabled, next)
		arrays[i++] = &tfile->tx_array;

	ret = skb_array_resize_multiple(arrays, n,
					dev->tx_queue_len, GFP_KERNEL);

	kfree(arrays);
	return ret;
}
