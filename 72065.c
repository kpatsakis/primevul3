void blk_account_io_done(struct request *req, u64 now)
{
	/*
	 * Account IO completion.  flush_rq isn't accounted as a
	 * normal IO on queueing nor completion.  Accounting the
	 * containing request is enough.
	 */
	if (blk_do_io_stat(req) && !(req->rq_flags & RQF_FLUSH_SEQ)) {
		unsigned long duration;
		const int sgrp = op_stat_group(req_op(req));
		struct hd_struct *part;
		int cpu;

		duration = nsecs_to_jiffies(now - req->start_time_ns);
		cpu = part_stat_lock();
		part = req->part;

		part_stat_inc(cpu, part, ios[sgrp]);
		part_stat_add(cpu, part, ticks[sgrp], duration);
		part_round_stats(req->q, cpu, part);
		part_dec_in_flight(req->q, part, rq_data_dir(req));

		hd_struct_put(part);
		part_stat_unlock();
	}
}
