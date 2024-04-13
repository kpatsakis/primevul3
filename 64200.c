    OVS_REQUIRES(ofproto_mutex)
{
    struct ofgroup *group;

    if (ogm->gm.group_id == OFPG_ALL) {
        CMAP_FOR_EACH (group, cmap_node, &ofproto->groups) {
            if (versions_visible_in_version(&group->versions, ogm->version)) {
                delete_group_start(ofproto, ogm->version, &ogm->old_groups,
                                   group);
            }
        }
    } else {
        group = ofproto_group_lookup__(ofproto, ogm->gm.group_id, ogm->version);
        if (group) {
            delete_group_start(ofproto, ogm->version, &ogm->old_groups, group);
        }
    }
}
