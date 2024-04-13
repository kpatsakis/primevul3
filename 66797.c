static int copy_msghdr_from_user(struct msghdr *kmsg,
				 struct user_msghdr __user *umsg,
				 struct sockaddr __user **save_addr,
				 struct iovec **iov)
{
	struct user_msghdr msg;
	ssize_t err;

	if (copy_from_user(&msg, umsg, sizeof(*umsg)))
		return -EFAULT;

	kmsg->msg_control = (void __force *)msg.msg_control;
	kmsg->msg_controllen = msg.msg_controllen;
	kmsg->msg_flags = msg.msg_flags;

	kmsg->msg_namelen = msg.msg_namelen;
	if (!msg.msg_name)
		kmsg->msg_namelen = 0;

	if (kmsg->msg_namelen < 0)
		return -EINVAL;

	if (kmsg->msg_namelen > sizeof(struct sockaddr_storage))
		kmsg->msg_namelen = sizeof(struct sockaddr_storage);

	if (save_addr)
		*save_addr = msg.msg_name;

	if (msg.msg_name && kmsg->msg_namelen) {
		if (!save_addr) {
			err = move_addr_to_kernel(msg.msg_name,
						  kmsg->msg_namelen,
						  kmsg->msg_name);
			if (err < 0)
				return err;
		}
	} else {
		kmsg->msg_name = NULL;
		kmsg->msg_namelen = 0;
	}

	if (msg.msg_iovlen > UIO_MAXIOV)
		return -EMSGSIZE;

	kmsg->msg_iocb = NULL;

	return import_iovec(save_addr ? READ : WRITE,
			    msg.msg_iov, msg.msg_iovlen,
			    UIO_FASTIOV, iov, &kmsg->msg_iter);
}
