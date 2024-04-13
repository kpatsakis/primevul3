parse_SET_L4_SRC_PORT(char *arg, struct ofpbuf *ofpacts,
                      enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    return str_to_u16(arg, "source port",
                      &ofpact_put_SET_L4_SRC_PORT(ofpacts)->port);
}
