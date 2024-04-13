parse_SET_MPLS_LABEL(char *arg, struct ofpbuf *ofpacts,
                     enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    struct ofpact_mpls_label *mpls_label = ofpact_put_SET_MPLS_LABEL(ofpacts);
    if (*arg == '\0') {
        return xstrdup("set_mpls_label: expected label.");
    }

    mpls_label->label = htonl(atoi(arg));
    return NULL;
}
