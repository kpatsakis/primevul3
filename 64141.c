    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofputil_flow_removed fr;
    long long int used;

    minimatch_expand(&rule->cr.match, &fr.match);
    fr.priority = rule->cr.priority;

    /* Synchronize with connmgr_destroy() calls to prevent connmgr disappearing
     * while we use it. */
    ovs_mutex_lock(&ofproto_mutex);
    struct connmgr *connmgr = rule->ofproto->connmgr;
    if (!connmgr) {
        ovs_mutex_unlock(&ofproto_mutex);
        return;
    }

    fr.cookie = rule->flow_cookie;
    fr.reason = rule->removed_reason;
    fr.table_id = rule->table_id;
    calc_duration(rule->created, time_msec(),
                  &fr.duration_sec, &fr.duration_nsec);
    ovs_mutex_lock(&rule->mutex);
    fr.idle_timeout = rule->idle_timeout;
    fr.hard_timeout = rule->hard_timeout;
    ovs_mutex_unlock(&rule->mutex);
    rule->ofproto->ofproto_class->rule_get_stats(rule, &fr.packet_count,
                                                 &fr.byte_count, &used);
    connmgr_send_flow_removed(connmgr, &fr);
    ovs_mutex_unlock(&ofproto_mutex);
}
