void CaptureStatsUpdate(ThreadVars *tv, CaptureStats *s, const Packet *p)
{
    if (unlikely(PACKET_TEST_ACTION(p, (ACTION_REJECT|ACTION_REJECT_DST|ACTION_REJECT_BOTH)))) {
        StatsIncr(tv, s->counter_ips_rejected);
    } else if (unlikely(PACKET_TEST_ACTION(p, ACTION_DROP))) {
        StatsIncr(tv, s->counter_ips_blocked);
    } else if (unlikely(p->flags & PKT_STREAM_MODIFIED)) {
        StatsIncr(tv, s->counter_ips_replaced);
    } else {
        StatsIncr(tv, s->counter_ips_accepted);
    }
}
