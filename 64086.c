parse_SET_IPV4_SRC(char *arg, struct ofpbuf *ofpacts,
                   enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    return str_to_ip(arg, &ofpact_put_SET_IPV4_SRC(ofpacts)->ipv4);
}
