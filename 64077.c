parse_OUTPUT_REG(const char *arg, struct ofpbuf *ofpacts,
                 enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    return parse_OUTPUT(arg, ofpacts, usable_protocols);
}
