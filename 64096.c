parse_SET_VLAN_PCP(char *arg, struct ofpbuf *ofpacts,
                   enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    return parse_set_vlan_pcp(arg, ofpacts, false);
}
