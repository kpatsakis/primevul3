static bool nohz_idle_balance(struct rq *this_rq, enum cpu_idle_type idle)
{
	int this_cpu = this_rq->cpu;
	unsigned int flags;

	if (!(atomic_read(nohz_flags(this_cpu)) & NOHZ_KICK_MASK))
		return false;

	if (idle != CPU_IDLE) {
		atomic_andnot(NOHZ_KICK_MASK, nohz_flags(this_cpu));
		return false;
	}

	/* could be _relaxed() */
	flags = atomic_fetch_andnot(NOHZ_KICK_MASK, nohz_flags(this_cpu));
	if (!(flags & NOHZ_KICK_MASK))
		return false;

	_nohz_idle_balance(this_rq, flags, idle);

	return true;
}
