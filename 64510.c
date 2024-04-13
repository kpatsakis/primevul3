ofputil_decode_role_message(const struct ofp_header *oh,
                            struct ofputil_role_request *rr)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);
    if (raw == OFPRAW_OFPT12_ROLE_REQUEST ||
        raw == OFPRAW_OFPT12_ROLE_REPLY) {
        const struct ofp12_role_request *orr = b.msg;

        if (orr->role != htonl(OFPCR12_ROLE_NOCHANGE) &&
            orr->role != htonl(OFPCR12_ROLE_EQUAL) &&
            orr->role != htonl(OFPCR12_ROLE_MASTER) &&
            orr->role != htonl(OFPCR12_ROLE_SLAVE)) {
            return OFPERR_OFPRRFC_BAD_ROLE;
        }

        rr->role = ntohl(orr->role);
        if (raw == OFPRAW_OFPT12_ROLE_REQUEST
            ? orr->role == htonl(OFPCR12_ROLE_NOCHANGE)
            : orr->generation_id == OVS_BE64_MAX) {
            rr->have_generation_id = false;
            rr->generation_id = 0;
        } else {
            rr->have_generation_id = true;
            rr->generation_id = ntohll(orr->generation_id);
        }
    } else if (raw == OFPRAW_NXT_ROLE_REQUEST ||
               raw == OFPRAW_NXT_ROLE_REPLY) {
        const struct nx_role_request *nrr = b.msg;

        BUILD_ASSERT(NX_ROLE_OTHER + 1 == OFPCR12_ROLE_EQUAL);
        BUILD_ASSERT(NX_ROLE_MASTER + 1 == OFPCR12_ROLE_MASTER);
        BUILD_ASSERT(NX_ROLE_SLAVE + 1 == OFPCR12_ROLE_SLAVE);

        if (nrr->role != htonl(NX_ROLE_OTHER) &&
            nrr->role != htonl(NX_ROLE_MASTER) &&
            nrr->role != htonl(NX_ROLE_SLAVE)) {
            return OFPERR_OFPRRFC_BAD_ROLE;
        }

        rr->role = ntohl(nrr->role) + 1;
        rr->have_generation_id = false;
        rr->generation_id = 0;
    } else {
        OVS_NOT_REACHED();
    }

    return 0;
}
