parse_SET_L4_DST_PORT(char *arg, struct ofpbuf *ofpacts,
                      enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    return str_to_u16(arg, "destination port",
                      &ofpact_put_SET_L4_DST_PORT(ofpacts)->port);
}
