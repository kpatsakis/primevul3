    OVS_EXCLUDED(ofproto_mutex)
{
    ovs_mutex_lock(&ofproto_mutex);
    remove_group_rcu__(group);
    ovs_mutex_unlock(&ofproto_mutex);
}
