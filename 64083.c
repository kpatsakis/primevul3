parse_SET_ETH_SRC(char *arg, struct ofpbuf *ofpacts,
                 enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    return str_to_mac(arg, &ofpact_put_SET_ETH_SRC(ofpacts)->mac);
}
