ofproto_group_lookup__(const struct ofproto *ofproto, uint32_t group_id,
                       ovs_version_t version)
{
    struct ofgroup *group;

    CMAP_FOR_EACH_WITH_HASH (group, cmap_node, hash_int(group_id, 0),
                             &ofproto->groups) {
        if (group->group_id == group_id
            && versions_visible_in_version(&group->versions, version)) {
            return group;
        }
    }

    return NULL;
}
