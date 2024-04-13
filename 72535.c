static unsigned long scale_rt_capacity(struct sched_domain *sd, int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	unsigned long max = arch_scale_cpu_capacity(sd, cpu);
	unsigned long used, free;
	unsigned long irq;

	irq = cpu_util_irq(rq);

	if (unlikely(irq >= max))
		return 1;

	used = READ_ONCE(rq->avg_rt.util_avg);
	used += READ_ONCE(rq->avg_dl.util_avg);

	if (unlikely(used >= max))
		return 1;

	free = max - used;

	return scale_irq_capacity(free, irq, max);
}
