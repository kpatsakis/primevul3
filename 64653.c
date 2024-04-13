ofputil_put_ofp11_match(struct ofpbuf *b, const struct match *match,
                        enum ofputil_protocol protocol)
{
    switch (protocol) {
    case OFPUTIL_P_OF10_STD:
    case OFPUTIL_P_OF10_STD_TID:
    case OFPUTIL_P_OF10_NXM:
    case OFPUTIL_P_OF10_NXM_TID:
        OVS_NOT_REACHED();

    case OFPUTIL_P_OF11_STD: {
        struct ofp11_match *om;

        /* Make sure that no padding is needed. */
        BUILD_ASSERT_DECL(sizeof *om % 8 == 0);

        om = ofpbuf_put_uninit(b, sizeof *om);
        ofputil_match_to_ofp11_match(match, om);
        return sizeof *om;
    }

    case OFPUTIL_P_OF12_OXM:
    case OFPUTIL_P_OF13_OXM:
    case OFPUTIL_P_OF14_OXM:
    case OFPUTIL_P_OF15_OXM:
    case OFPUTIL_P_OF16_OXM:
        return oxm_put_match(b, match,
                             ofputil_protocol_to_ofp_version(protocol));
    }

    OVS_NOT_REACHED();
}
