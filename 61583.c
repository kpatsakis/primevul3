static void _delay_rpc(int host_inx, int host_cnt, int usec_per_rpc)
{
	struct timeval tv1;
	uint32_t cur_time;	/* current time in usec (just 9 digits) */
	uint32_t tot_time;	/* total time expected for all RPCs */
	uint32_t offset_time;	/* relative time within tot_time */
	uint32_t target_time;	/* desired time to issue the RPC */
	uint32_t delta_time;

again:	if (gettimeofday(&tv1, NULL)) {
		usleep(host_inx * usec_per_rpc);
		return;
	}

	cur_time = ((tv1.tv_sec % 1000) * 1000000) + tv1.tv_usec;
	tot_time = host_cnt * usec_per_rpc;
	offset_time = cur_time % tot_time;
	target_time = host_inx * usec_per_rpc;
	if (target_time < offset_time)
		delta_time = target_time - offset_time + tot_time;
	else
		delta_time = target_time - offset_time;
	if (usleep(delta_time)) {
		if (errno == EINVAL) /* usleep for more than 1 sec */
			usleep(900000);
		/* errno == EINTR */
		goto again;
	}
}
