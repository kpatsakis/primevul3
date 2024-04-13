parse_SET_MPLS_TC(char *arg, struct ofpbuf *ofpacts,
                  enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    struct ofpact_mpls_tc *mpls_tc = ofpact_put_SET_MPLS_TC(ofpacts);

    if (*arg == '\0') {
        return xstrdup("set_mpls_tc: expected tc.");
    }

    mpls_tc->tc = atoi(arg);
    return NULL;
}
