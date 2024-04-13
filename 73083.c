static struct sk_buff *tun_ring_recv(struct tun_file *tfile, int noblock,
				     int *err)
{
	DECLARE_WAITQUEUE(wait, current);
	struct sk_buff *skb = NULL;
	int error = 0;

	skb = skb_array_consume(&tfile->tx_array);
	if (skb)
		goto out;
	if (noblock) {
		error = -EAGAIN;
		goto out;
	}

	add_wait_queue(&tfile->wq.wait, &wait);
	current->state = TASK_INTERRUPTIBLE;

	while (1) {
		skb = skb_array_consume(&tfile->tx_array);
		if (skb)
			break;
		if (signal_pending(current)) {
			error = -ERESTARTSYS;
			break;
		}
		if (tfile->socket.sk->sk_shutdown & RCV_SHUTDOWN) {
			error = -EFAULT;
			break;
		}

		schedule();
	}

	current->state = TASK_RUNNING;
	remove_wait_queue(&tfile->wq.wait, &wait);

out:
	*err = error;
	return skb;
}
