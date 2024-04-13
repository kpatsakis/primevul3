    OVS_REQUIRES(ofproto_mutex)
{
    /* Calculate absolute time when this flow will expire.  If it will never
     * expire, then return 0 to make it unevictable.  */
    long long int expiration = LLONG_MAX;
    if (rule->hard_timeout) {
        /* 'modified' needs protection even when we hold 'ofproto_mutex'. */
        ovs_mutex_lock(&rule->mutex);
        long long int modified = rule->modified;
        ovs_mutex_unlock(&rule->mutex);

        expiration = modified + rule->hard_timeout * 1000;
    }
    if (rule->idle_timeout) {
        uint64_t packets, bytes;
        long long int used;
        long long int idle_expiration;

        ofproto->ofproto_class->rule_get_stats(rule, &packets, &bytes, &used);
        idle_expiration = used + rule->idle_timeout * 1000;
        expiration = MIN(expiration, idle_expiration);
    }
    if (expiration == LLONG_MAX) {
        return 0;
    }

    /* Calculate the time of expiration as a number of (approximate) seconds
     * after program startup.
     *
     * This should work OK for program runs that last UINT32_MAX seconds or
     * less.  Therefore, please restart OVS at least once every 136 years. */
    uint32_t expiration_ofs = (expiration >> 10) - (time_boot_msec() >> 10);

    /* Combine expiration time with OpenFlow "importance" to form a single
     * priority value.  We want flows with relatively low "importance" to be
     * evicted before even considering expiration time, so put "importance" in
     * the most significant bits and expiration time in the least significant
     * bits.
     *
     * Small 'priority' should be evicted before those with large 'priority'.
     * The caller expects the opposite convention (a large return value being
     * more attractive for eviction) so we invert it before returning. */
    uint64_t priority = ((uint64_t) rule->importance << 32) + expiration_ofs;
    return UINT64_MAX - priority;
}
