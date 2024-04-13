ofputil_decode_queue_stats(struct ofputil_queue_stats *qs, struct ofpbuf *msg)
{
    enum ofperr error;
    enum ofpraw raw;

    error = (msg->header ? ofpraw_decode(&raw, msg->header)
             : ofpraw_pull(&raw, msg));
    if (error) {
        return error;
    }

    if (!msg->size) {
        return EOF;
    } else if (raw == OFPRAW_OFPST14_QUEUE_REPLY) {
        return ofputil_pull_ofp14_queue_stats(qs, msg);
    } else if (raw == OFPRAW_OFPST13_QUEUE_REPLY) {
        const struct ofp13_queue_stats *qs13;

        qs13 = ofpbuf_try_pull(msg, sizeof *qs13);
        if (!qs13) {
            goto bad_len;
        }
        return ofputil_queue_stats_from_ofp13(qs, qs13);
    } else if (raw == OFPRAW_OFPST11_QUEUE_REPLY) {
        const struct ofp11_queue_stats *qs11;

        qs11 = ofpbuf_try_pull(msg, sizeof *qs11);
        if (!qs11) {
            goto bad_len;
        }
        return ofputil_queue_stats_from_ofp11(qs, qs11);
    } else if (raw == OFPRAW_OFPST10_QUEUE_REPLY) {
        const struct ofp10_queue_stats *qs10;

        qs10 = ofpbuf_try_pull(msg, sizeof *qs10);
        if (!qs10) {
            goto bad_len;
        }
        return ofputil_queue_stats_from_ofp10(qs, qs10);
    } else {
        OVS_NOT_REACHED();
    }

 bad_len:
    VLOG_WARN_RL(&bad_ofmsg_rl, "OFPST_QUEUE reply has %"PRIu32" leftover "
                 "bytes at end", msg->size);
    return OFPERR_OFPBRC_BAD_LEN;
}
