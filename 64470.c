ofputil_decode_flow_monitor_request(struct ofputil_flow_monitor_request *rq,
                                    struct ofpbuf *msg)
{
    struct nx_flow_monitor_request *nfmr;
    uint16_t flags;

    if (!msg->header) {
        ofpraw_pull_assert(msg);
    }

    if (!msg->size) {
        return EOF;
    }

    nfmr = ofpbuf_try_pull(msg, sizeof *nfmr);
    if (!nfmr) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "NXST_FLOW_MONITOR request has %"PRIu32" "
                     "leftover bytes at end", msg->size);
        return OFPERR_OFPBRC_BAD_LEN;
    }

    flags = ntohs(nfmr->flags);
    if (!(flags & (NXFMF_ADD | NXFMF_DELETE | NXFMF_MODIFY))
        || flags & ~(NXFMF_INITIAL | NXFMF_ADD | NXFMF_DELETE
                     | NXFMF_MODIFY | NXFMF_ACTIONS | NXFMF_OWN)) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "NXST_FLOW_MONITOR has bad flags %#"PRIx16,
                     flags);
        return OFPERR_OFPMOFC_BAD_FLAGS;
    }

    if (!is_all_zeros(nfmr->zeros, sizeof nfmr->zeros)) {
        return OFPERR_NXBRC_MUST_BE_ZERO;
    }

    rq->id = ntohl(nfmr->id);
    rq->flags = flags;
    rq->out_port = u16_to_ofp(ntohs(nfmr->out_port));
    rq->table_id = nfmr->table_id;

    return nx_pull_match(msg, ntohs(nfmr->match_len), &rq->match, NULL,
                         NULL, NULL, NULL);
}
