parse_WRITE_METADATA(char *arg, struct ofpbuf *ofpacts,
                     enum ofputil_protocol *usable_protocols)
{
    struct ofpact_metadata *om;
    char *mask = strchr(arg, '/');

    *usable_protocols &= OFPUTIL_P_NXM_OF11_UP;

    om = ofpact_put_WRITE_METADATA(ofpacts);
    if (mask) {
        char *error;

        *mask = '\0';
        error = str_to_be64(mask + 1, &om->mask);
        if (error) {
            return error;
        }
    } else {
        om->mask = OVS_BE64_MAX;
    }

    return str_to_be64(arg, &om->metadata);
}
