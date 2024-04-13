static void tun_sock_write_space(struct sock *sk)
{
	struct tun_file *tfile;
	wait_queue_head_t *wqueue;

	if (!sock_writeable(sk))
		return;

	if (!test_and_clear_bit(SOCKWQ_ASYNC_NOSPACE, &sk->sk_socket->flags))
		return;

	wqueue = sk_sleep(sk);
	if (wqueue && waitqueue_active(wqueue))
		wake_up_interruptible_sync_poll(wqueue, POLLOUT |
						POLLWRNORM | POLLWRBAND);

	tfile = container_of(sk, struct tun_file, sk);
	kill_fasync(&tfile->fasync, SIGIO, POLL_OUT);
}
