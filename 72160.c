void part_round_stats(struct request_queue *q, int cpu, struct hd_struct *part)
{
	struct hd_struct *part2 = NULL;
	unsigned long now = jiffies;
	unsigned int inflight[2];
	int stats = 0;

	if (part->stamp != now)
		stats |= 1;

	if (part->partno) {
		part2 = &part_to_disk(part)->part0;
		if (part2->stamp != now)
			stats |= 2;
	}

	if (!stats)
		return;

	part_in_flight(q, part, inflight);

	if (stats & 2)
		part_round_stats_single(q, cpu, part2, now, inflight[1]);
	if (stats & 1)
		part_round_stats_single(q, cpu, part, now, inflight[0]);
}
