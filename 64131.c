    OVS_EXCLUDED(ofproto_mutex)
{
    ovs_mutex_lock(&ofproto_mutex);
    remove_rule_rcu__(rule);
    ovs_mutex_unlock(&ofproto_mutex);
}
