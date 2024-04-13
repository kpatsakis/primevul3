static ssize_t tun_chr_write_iter(struct kiocb *iocb, struct iov_iter *from)
{
	struct file *file = iocb->ki_filp;
	struct tun_struct *tun = tun_get(file);
	struct tun_file *tfile = file->private_data;
	ssize_t result;

	if (!tun)
		return -EBADFD;

	result = tun_get_user(tun, tfile, NULL, from,
			      file->f_flags & O_NONBLOCK, false);

	tun_put(tun);
	return result;
}
