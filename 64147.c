    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofgroup *group;
    struct ovs_list replies;

    ofpmp_init(&replies, request);
    /* Must exclude modifications to guarantee iterating groups. */
    ovs_mutex_lock(&ofproto_mutex);
    if (group_id == OFPG_ALL) {
        CMAP_FOR_EACH (group, cmap_node, &ofproto->groups) {
            if (versions_visible_in_version(&group->versions,
                                            OVS_VERSION_MAX)) {
                cb(group, &replies);
            }
        }
    } else {
        group = ofproto_group_lookup__(ofproto, group_id, OVS_VERSION_MAX);
        if (group) {
            cb(group, &replies);
        }
    }
    ovs_mutex_unlock(&ofproto_mutex);
    ofconn_send_replies(ofconn, &replies);
}
