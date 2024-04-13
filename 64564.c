ofputil_encode_role_reply(const struct ofp_header *request,
                          const struct ofputil_role_request *rr)
{
    struct ofpbuf *buf;
    enum ofpraw raw;

    raw = ofpraw_decode_assert(request);
    if (raw == OFPRAW_OFPT12_ROLE_REQUEST) {
        struct ofp12_role_request *orr;

        buf = ofpraw_alloc_reply(OFPRAW_OFPT12_ROLE_REPLY, request, 0);
        orr = ofpbuf_put_zeros(buf, sizeof *orr);

        orr->role = htonl(rr->role);
        orr->generation_id = htonll(rr->have_generation_id
                                    ? rr->generation_id
                                    : UINT64_MAX);
    } else if (raw == OFPRAW_NXT_ROLE_REQUEST) {
        struct nx_role_request *nrr;

        BUILD_ASSERT(NX_ROLE_OTHER == OFPCR12_ROLE_EQUAL - 1);
        BUILD_ASSERT(NX_ROLE_MASTER == OFPCR12_ROLE_MASTER - 1);
        BUILD_ASSERT(NX_ROLE_SLAVE == OFPCR12_ROLE_SLAVE - 1);

        buf = ofpraw_alloc_reply(OFPRAW_NXT_ROLE_REPLY, request, 0);
        nrr = ofpbuf_put_zeros(buf, sizeof *nrr);
        nrr->role = htonl(rr->role - 1);
    } else {
        OVS_NOT_REACHED();
    }

    return buf;
}
