parse_SET_TUNNEL(char *arg, struct ofpbuf *ofpacts,
                 enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    return parse_set_tunnel(arg, ofpacts, NXAST_RAW_SET_TUNNEL);
}
