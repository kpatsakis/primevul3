    OVS_REQUIRES(ofproto_mutex)
{
    struct ofproto *ofproto = group->ofproto;

    ovs_assert(!versions_visible_in_version(&group->versions, OVS_VERSION_MAX));

    cmap_remove(&ofproto->groups, &group->cmap_node,
                hash_int(group->group_id, 0));
    ofproto_group_unref(group);
}
