ofpact_parse(enum ofpact_type type, char *value, struct ofpbuf *ofpacts,
             enum ofputil_protocol *usable_protocols)
{
    switch (type) {
#define OFPACT(ENUM, STRUCT, MEMBER, NAME)                            \
        case OFPACT_##ENUM:                                             \
            return parse_##ENUM(value, ofpacts, usable_protocols);
        OFPACTS
#undef OFPACT
    default:
        OVS_NOT_REACHED();
    }
}
