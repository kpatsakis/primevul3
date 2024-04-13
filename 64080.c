parse_PUSH_MPLS(char *arg, struct ofpbuf *ofpacts,
                enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    uint16_t ethertype;
    char *error;

    error = str_to_u16(arg, "push_mpls", &ethertype);
    if (!error) {
        ofpact_put_PUSH_MPLS(ofpacts)->ethertype = htons(ethertype);
    }
    return error;
}
