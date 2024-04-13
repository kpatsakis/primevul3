struct skb_array *tun_get_skb_array(struct file *file)
{
	struct tun_file *tfile;

	if (file->f_op != &tun_fops)
		return ERR_PTR(-EINVAL);
	tfile = file->private_data;
	if (!tfile)
		return ERR_PTR(-EBADFD);
	return &tfile->tx_array;
}
