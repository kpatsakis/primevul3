int __sys_socketpair(int family, int type, int protocol, int __user *usockvec)
{
	struct socket *sock1, *sock2;
	int fd1, fd2, err;
	struct file *newfile1, *newfile2;
	int flags;

	flags = type & ~SOCK_TYPE_MASK;
	if (flags & ~(SOCK_CLOEXEC | SOCK_NONBLOCK))
		return -EINVAL;
	type &= SOCK_TYPE_MASK;

	if (SOCK_NONBLOCK != O_NONBLOCK && (flags & SOCK_NONBLOCK))
		flags = (flags & ~SOCK_NONBLOCK) | O_NONBLOCK;

	/*
	 * reserve descriptors and make sure we won't fail
	 * to return them to userland.
	 */
	fd1 = get_unused_fd_flags(flags);
	if (unlikely(fd1 < 0))
		return fd1;

	fd2 = get_unused_fd_flags(flags);
	if (unlikely(fd2 < 0)) {
		put_unused_fd(fd1);
		return fd2;
	}

	err = put_user(fd1, &usockvec[0]);
	if (err)
		goto out;

	err = put_user(fd2, &usockvec[1]);
	if (err)
		goto out;

	/*
	 * Obtain the first socket and check if the underlying protocol
	 * supports the socketpair call.
	 */

	err = sock_create(family, type, protocol, &sock1);
	if (unlikely(err < 0))
		goto out;

	err = sock_create(family, type, protocol, &sock2);
	if (unlikely(err < 0)) {
		sock_release(sock1);
		goto out;
	}

	err = security_socket_socketpair(sock1, sock2);
	if (unlikely(err)) {
		sock_release(sock2);
		sock_release(sock1);
		goto out;
	}

	err = sock1->ops->socketpair(sock1, sock2);
	if (unlikely(err < 0)) {
		sock_release(sock2);
		sock_release(sock1);
		goto out;
	}

	newfile1 = sock_alloc_file(sock1, flags, NULL);
	if (IS_ERR(newfile1)) {
		err = PTR_ERR(newfile1);
		sock_release(sock2);
		goto out;
	}

	newfile2 = sock_alloc_file(sock2, flags, NULL);
	if (IS_ERR(newfile2)) {
		err = PTR_ERR(newfile2);
		fput(newfile1);
		goto out;
	}

	audit_fd_pair(fd1, fd2);

	fd_install(fd1, newfile1);
	fd_install(fd2, newfile2);
	return 0;

out:
	put_unused_fd(fd2);
	put_unused_fd(fd1);
	return err;
}
