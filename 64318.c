ofproto_group_exists(const struct ofproto *ofproto, uint32_t group_id)
{
    return ofproto_group_lookup__(ofproto, group_id, OVS_VERSION_MAX) != NULL;
}
