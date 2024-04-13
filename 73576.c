xfs_vm_direct_IO(
	struct kiocb		*iocb,
	struct iov_iter		*iter)
{
	/*
	 * We just need the method present so that open/fcntl allow direct I/O.
	 */
	return -EINVAL;
}
