parse_SET_IP_DSCP(char *arg, struct ofpbuf *ofpacts,
                 enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    uint8_t tos;
    char *error;

    error = str_to_u8(arg, "TOS", &tos);
    if (error) {
        return error;
    }

    if (tos & ~IP_DSCP_MASK) {
        return xasprintf("%s: not a valid TOS", arg);
    }
    ofpact_put_SET_IP_DSCP(ofpacts)->dscp = tos;
    return NULL;
}
