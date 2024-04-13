int kernel_sendpage_locked(struct sock *sk, struct page *page, int offset,
			   size_t size, int flags)
{
	struct socket *sock = sk->sk_socket;

	if (sock->ops->sendpage_locked)
		return sock->ops->sendpage_locked(sk, page, offset, size,
						  flags);

	return sock_no_sendpage_locked(sk, page, offset, size, flags);
}
