static int __dequeue_signal(struct sigpending *pending, sigset_t *mask,
			siginfo_t *info, bool *resched_timer)
{
	int sig = next_signal(pending, mask);

	if (sig)
		collect_signal(sig, pending, info, resched_timer);
	return sig;
}
