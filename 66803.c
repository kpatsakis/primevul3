int kernel_sendmsg_locked(struct sock *sk, struct msghdr *msg,
			  struct kvec *vec, size_t num, size_t size)
{
	struct socket *sock = sk->sk_socket;

	if (!sock->ops->sendmsg_locked)
		return sock_no_sendmsg_locked(sk, msg, size);

	iov_iter_kvec(&msg->msg_iter, WRITE | ITER_KVEC, vec, num, size);

	return sock->ops->sendmsg_locked(sk, msg, msg_data_left(msg));
}
