ofputil_decode_role_status(const struct ofp_header *oh,
                           struct ofputil_role_status *rs)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);
    ovs_assert(raw == OFPRAW_OFPT14_ROLE_STATUS);

    const struct ofp14_role_status *r = b.msg;
    if (r->role != htonl(OFPCR12_ROLE_NOCHANGE) &&
        r->role != htonl(OFPCR12_ROLE_EQUAL) &&
        r->role != htonl(OFPCR12_ROLE_MASTER) &&
        r->role != htonl(OFPCR12_ROLE_SLAVE)) {
        return OFPERR_OFPRRFC_BAD_ROLE;
    }

    rs->role = ntohl(r->role);
    rs->generation_id = ntohll(r->generation_id);
    rs->reason = r->reason;

    return 0;
}
