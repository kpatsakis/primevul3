    OVS_REQUIRES(ofproto_mutex)
{
    if (group_collection_n(groups) > 0) {
        if (group_collection_n(groups) == 1) {
            ovsrcu_postpone(remove_group_rcu,
                            group_collection_groups(groups)[0]);
            group_collection_init(groups);
        } else {
            ovsrcu_postpone(remove_groups_rcu,
                            group_collection_detach(groups));
        }
    }
}
