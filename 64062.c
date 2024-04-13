parse_BUNDLE(const char *arg, struct ofpbuf *ofpacts,
             enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    return bundle_parse(arg, ofpacts);
}
