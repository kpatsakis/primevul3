void CaptureStatsSetup(ThreadVars *tv, CaptureStats *s)
{
    s->counter_ips_accepted = StatsRegisterCounter("ips.accepted", tv);
    s->counter_ips_blocked = StatsRegisterCounter("ips.blocked", tv);
    s->counter_ips_rejected = StatsRegisterCounter("ips.rejected", tv);
    s->counter_ips_replaced = StatsRegisterCounter("ips.replaced", tv);
}
