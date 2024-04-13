ofputil_decode_flow_removed(struct ofputil_flow_removed *fr,
                            const struct ofp_header *oh)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);
    if (raw == OFPRAW_OFPT11_FLOW_REMOVED) {
        const struct ofp12_flow_removed *ofr;
        enum ofperr error;

        ofr = ofpbuf_pull(&b, sizeof *ofr);

        error = ofputil_pull_ofp11_match(&b, NULL, NULL, &fr->match, NULL);
        if (error) {
            return error;
        }

        fr->priority = ntohs(ofr->priority);
        fr->cookie = ofr->cookie;
        fr->reason = ofr->reason;
        fr->table_id = ofr->table_id;
        fr->duration_sec = ntohl(ofr->duration_sec);
        fr->duration_nsec = ntohl(ofr->duration_nsec);
        fr->idle_timeout = ntohs(ofr->idle_timeout);
        fr->hard_timeout = ntohs(ofr->hard_timeout);
        fr->packet_count = ntohll(ofr->packet_count);
        fr->byte_count = ntohll(ofr->byte_count);
    } else if (raw == OFPRAW_OFPT10_FLOW_REMOVED) {
        const struct ofp10_flow_removed *ofr;

        ofr = ofpbuf_pull(&b, sizeof *ofr);

        ofputil_match_from_ofp10_match(&ofr->match, &fr->match);
        fr->priority = ntohs(ofr->priority);
        fr->cookie = ofr->cookie;
        fr->reason = ofr->reason;
        fr->table_id = 255;
        fr->duration_sec = ntohl(ofr->duration_sec);
        fr->duration_nsec = ntohl(ofr->duration_nsec);
        fr->idle_timeout = ntohs(ofr->idle_timeout);
        fr->hard_timeout = 0;
        fr->packet_count = ntohll(ofr->packet_count);
        fr->byte_count = ntohll(ofr->byte_count);
    } else if (raw == OFPRAW_NXT_FLOW_REMOVED) {
        struct nx_flow_removed *nfr;
        enum ofperr error;

        nfr = ofpbuf_pull(&b, sizeof *nfr);
        error = nx_pull_match(&b, ntohs(nfr->match_len), &fr->match, NULL,
                              NULL, NULL, NULL);
        if (error) {
            return error;
        }
        if (b.size) {
            return OFPERR_OFPBRC_BAD_LEN;
        }

        fr->priority = ntohs(nfr->priority);
        fr->cookie = nfr->cookie;
        fr->reason = nfr->reason;
        fr->table_id = nfr->table_id ? nfr->table_id - 1 : 255;
        fr->duration_sec = ntohl(nfr->duration_sec);
        fr->duration_nsec = ntohl(nfr->duration_nsec);
        fr->idle_timeout = ntohs(nfr->idle_timeout);
        fr->hard_timeout = 0;
        fr->packet_count = ntohll(nfr->packet_count);
        fr->byte_count = ntohll(nfr->byte_count);
    } else {
        OVS_NOT_REACHED();
    }

    return 0;
}
