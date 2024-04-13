ofputil_decode_flow_update(struct ofputil_flow_update *update,
                           struct ofpbuf *msg, struct ofpbuf *ofpacts)
{
    struct nx_flow_update_header *nfuh;
    unsigned int length;
    struct ofp_header *oh;

    if (!msg->header) {
        ofpraw_pull_assert(msg);
    }

    ofpbuf_clear(ofpacts);
    if (!msg->size) {
        return EOF;
    }

    if (msg->size < sizeof(struct nx_flow_update_header)) {
        goto bad_len;
    }

    oh = msg->header;

    nfuh = msg->data;
    update->event = ntohs(nfuh->event);
    length = ntohs(nfuh->length);
    if (length > msg->size || length % 8) {
        goto bad_len;
    }

    if (update->event == NXFME_ABBREV) {
        struct nx_flow_update_abbrev *nfua;

        if (length != sizeof *nfua) {
            goto bad_len;
        }

        nfua = ofpbuf_pull(msg, sizeof *nfua);
        update->xid = nfua->xid;
        return 0;
    } else if (update->event == NXFME_ADDED
               || update->event == NXFME_DELETED
               || update->event == NXFME_MODIFIED) {
        struct nx_flow_update_full *nfuf;
        unsigned int actions_len;
        unsigned int match_len;
        enum ofperr error;

        if (length < sizeof *nfuf) {
            goto bad_len;
        }

        nfuf = ofpbuf_pull(msg, sizeof *nfuf);
        match_len = ntohs(nfuf->match_len);
        if (sizeof *nfuf + match_len > length) {
            goto bad_len;
        }

        update->reason = ntohs(nfuf->reason);
        update->idle_timeout = ntohs(nfuf->idle_timeout);
        update->hard_timeout = ntohs(nfuf->hard_timeout);
        update->table_id = nfuf->table_id;
        update->cookie = nfuf->cookie;
        update->priority = ntohs(nfuf->priority);

        error = nx_pull_match(msg, match_len, &update->match, NULL, NULL, NULL,
                              NULL);
        if (error) {
            return error;
        }

        actions_len = length - sizeof *nfuf - ROUND_UP(match_len, 8);
        error = ofpacts_pull_openflow_actions(msg, actions_len, oh->version,
                                              NULL, NULL, ofpacts);
        if (error) {
            return error;
        }

        update->ofpacts = ofpacts->data;
        update->ofpacts_len = ofpacts->size;
        return 0;
    } else {
        VLOG_WARN_RL(&bad_ofmsg_rl,
                     "NXST_FLOW_MONITOR reply has bad event %"PRIu16,
                     ntohs(nfuh->event));
        return OFPERR_NXBRC_FM_BAD_EVENT;
    }

bad_len:
    VLOG_WARN_RL(&bad_ofmsg_rl, "NXST_FLOW_MONITOR reply has %"PRIu32" "
                 "leftover bytes at end", msg->size);
    return OFPERR_OFPBRC_BAD_LEN;
}
