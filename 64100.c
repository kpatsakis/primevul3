parse_STRIP_VLAN(char *arg OVS_UNUSED, struct ofpbuf *ofpacts,
                 enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    ofpact_put_STRIP_VLAN(ofpacts)->ofpact.raw = OFPAT_RAW10_STRIP_VLAN;
    return NULL;
}
