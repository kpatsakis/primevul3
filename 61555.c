static int aio_setup_vectored_rw(int rw, char __user *buf, size_t len,
				 struct iovec **iovec,
				 bool compat,
				 struct iov_iter *iter)
{
#ifdef CONFIG_COMPAT
	if (compat)
		return compat_import_iovec(rw,
				(struct compat_iovec __user *)buf,
				len, UIO_FASTIOV, iovec, iter);
#endif
	return import_iovec(rw, (struct iovec __user *)buf,
				len, UIO_FASTIOV, iovec, iter);
}
