parse_DEC_MPLS_TTL(char *arg OVS_UNUSED, struct ofpbuf *ofpacts,
                   enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    ofpact_put_DEC_MPLS_TTL(ofpacts);
    return NULL;
}
