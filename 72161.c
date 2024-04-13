static void part_round_stats_single(struct request_queue *q, int cpu,
				    struct hd_struct *part, unsigned long now,
				    unsigned int inflight)
{
	if (inflight) {
		__part_stat_add(cpu, part, time_in_queue,
				inflight * (now - part->stamp));
		__part_stat_add(cpu, part, io_ticks, (now - part->stamp));
	}
	part->stamp = now;
}
