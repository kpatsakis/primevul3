parse_MULTIPATH(const char *arg, struct ofpbuf *ofpacts,
                enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    return multipath_parse(ofpact_put_MULTIPATH(ofpacts), arg);
}
