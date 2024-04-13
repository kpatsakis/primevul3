parse_SET_IP_ECN(char *arg, struct ofpbuf *ofpacts,
                 enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    uint8_t ecn;
    char *error;

    error = str_to_u8(arg, "ECN", &ecn);
    if (error) {
        return error;
    }

    if (ecn & ~IP_ECN_MASK) {
        return xasprintf("%s: not a valid ECN", arg);
    }
    ofpact_put_SET_IP_ECN(ofpacts)->ecn = ecn;
    return NULL;
}
