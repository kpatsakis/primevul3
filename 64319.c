ofproto_group_lookup(const struct ofproto *ofproto, uint32_t group_id,
                     ovs_version_t version, bool take_ref)
{
    struct ofgroup *group;

    group = ofproto_group_lookup__(ofproto, group_id, version);
    if (group && take_ref) {
        /* Not holding a lock, so it is possible that another thread releases
         * the last reference just before we manage to get one. */
        return ofproto_group_try_ref(group) ? group : NULL;
    }
    return group;
}
