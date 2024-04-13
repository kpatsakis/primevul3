parse_action_bitmap(struct ofpbuf *payload, enum ofp_version ofp_version,
                    uint64_t *ofpacts)
{
    uint32_t types = 0;

    while (payload->size > 0) {
        enum ofperr error;
        uint64_t type;

        error = ofpprop_pull__(payload, NULL, 1, 0x10000, &type);
        if (error) {
            return error;
        }
        if (type < CHAR_BIT * sizeof types) {
            types |= 1u << type;
        }
    }

    *ofpacts = ofpact_bitmap_from_openflow(htonl(types), ofp_version);
    return 0;
}
