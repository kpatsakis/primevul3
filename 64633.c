ofputil_protocols_to_string(enum ofputil_protocol protocols)
{
    struct ds s;

    ovs_assert(!(protocols & ~OFPUTIL_P_ANY));
    if (protocols == 0) {
        return xstrdup("none");
    }

    ds_init(&s);
    while (protocols) {
        const struct proto_abbrev *p;
        int i;

        if (s.length) {
            ds_put_char(&s, ',');
        }

        for (p = proto_abbrevs; p < &proto_abbrevs[N_PROTO_ABBREVS]; p++) {
            if ((protocols & p->protocol) == p->protocol) {
                ds_put_cstr(&s, p->name);
                protocols &= ~p->protocol;
                goto match;
            }
        }

        for (i = 0; i < CHAR_BIT * sizeof(enum ofputil_protocol); i++) {
            enum ofputil_protocol bit = 1u << i;

            if (protocols & bit) {
                ds_put_cstr(&s, ofputil_protocol_to_string(bit));
                protocols &= ~bit;
                goto match;
            }
        }
        OVS_NOT_REACHED();

    match: ;
    }
    return ds_steal_cstr(&s);
}
