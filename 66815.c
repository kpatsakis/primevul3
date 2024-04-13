static __poll_t sock_poll(struct file *file, poll_table *wait)
{
	struct socket *sock = file->private_data;
	__poll_t events = poll_requested_events(wait), mask = 0;

	if (sock->ops->poll) {
		sock_poll_busy_loop(sock, events);
		mask = sock->ops->poll(file, sock, wait);
	} else if (sock->ops->poll_mask) {
		sock_poll_wait(file, sock_get_poll_head(file, events), wait);
		mask = sock->ops->poll_mask(sock, events);
	}

	return mask | sock_poll_busy_flag(sock);
}
