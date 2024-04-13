    OVS_REQUIRES(ofproto_mutex)
    OVS_EXCLUDED(rule->mutex)
{
    if (!idle_timeout && !hard_timeout) {
        return;
    }

    if (ovs_list_is_empty(&rule->expirable)) {
        ovs_list_insert(&rule->ofproto->expirable, &rule->expirable);
    }

    ovs_mutex_lock(&rule->mutex);
    reduce_timeout(idle_timeout, &rule->idle_timeout);
    reduce_timeout(hard_timeout, &rule->hard_timeout);
    ovs_mutex_unlock(&rule->mutex);
}
