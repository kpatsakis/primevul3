static void stats_reset(void) {
    STATS_LOCK();
    memset(&stats, 0, sizeof(struct stats));
    stats_prefix_clear();
    STATS_UNLOCK();
    threadlocal_stats_reset();
    item_stats_reset();
}
