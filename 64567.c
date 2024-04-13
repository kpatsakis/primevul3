ofputil_encode_set_protocol(enum ofputil_protocol current,
                            enum ofputil_protocol want,
                            enum ofputil_protocol *next)
{
    enum ofp_version cur_version, want_version;
    enum ofputil_protocol cur_base, want_base;
    bool cur_tid, want_tid;

    cur_version = ofputil_protocol_to_ofp_version(current);
    want_version = ofputil_protocol_to_ofp_version(want);
    if (cur_version != want_version) {
        *next = current;
        return NULL;
    }

    cur_base = ofputil_protocol_to_base(current);
    want_base = ofputil_protocol_to_base(want);
    if (cur_base != want_base) {
        *next = ofputil_protocol_set_base(current, want_base);

        switch (want_base) {
        case OFPUTIL_P_OF10_NXM:
            return ofputil_encode_nx_set_flow_format(NXFF_NXM);

        case OFPUTIL_P_OF10_STD:
            return ofputil_encode_nx_set_flow_format(NXFF_OPENFLOW10);

        case OFPUTIL_P_OF11_STD:
        case OFPUTIL_P_OF12_OXM:
        case OFPUTIL_P_OF13_OXM:
        case OFPUTIL_P_OF14_OXM:
        case OFPUTIL_P_OF15_OXM:
        case OFPUTIL_P_OF16_OXM:
            /* There is only one variant of each OpenFlow 1.1+ protocol, and we
             * verified above that we're not trying to change versions. */
            OVS_NOT_REACHED();

        case OFPUTIL_P_OF10_STD_TID:
        case OFPUTIL_P_OF10_NXM_TID:
            OVS_NOT_REACHED();
        }
    }

    cur_tid = (current & OFPUTIL_P_TID) != 0;
    want_tid = (want & OFPUTIL_P_TID) != 0;
    if (cur_tid != want_tid) {
        *next = ofputil_protocol_set_tid(current, want_tid);
        return ofputil_make_flow_mod_table_id(want_tid);
    }

    ovs_assert(current == want);

    *next = current;
    return NULL;
}
