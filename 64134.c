    OVS_EXCLUDED(ofproto_mutex)
{
    ovs_mutex_lock(&ofproto_mutex);
    for (struct ofgroup **g = groups; *g; g++) {
        remove_group_rcu__(*g);
    }
    ovs_mutex_unlock(&ofproto_mutex);
    free(groups);
}
