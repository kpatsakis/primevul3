static ssize_t tun_chr_read_iter(struct kiocb *iocb, struct iov_iter *to)
{
	struct file *file = iocb->ki_filp;
	struct tun_file *tfile = file->private_data;
	struct tun_struct *tun = __tun_get(tfile);
	ssize_t len = iov_iter_count(to), ret;

	if (!tun)
		return -EBADFD;
	ret = tun_do_read(tun, tfile, to, file->f_flags & O_NONBLOCK, NULL);
	ret = min_t(ssize_t, ret, len);
	if (ret > 0)
		iocb->ki_pos = ret;
	tun_put(tun);
	return ret;
}
