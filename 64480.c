ofputil_decode_group_stats_reply(struct ofpbuf *msg,
                                 struct ofputil_group_stats *gs)
{
    struct ofp11_bucket_counter *obc;
    struct ofp11_group_stats *ogs11;
    enum ofpraw raw;
    enum ofperr error;
    size_t base_len;
    size_t length;
    size_t i;

    gs->bucket_stats = NULL;
    error = (msg->header ? ofpraw_decode(&raw, msg->header)
             : ofpraw_pull(&raw, msg));
    if (error) {
        return error;
    }

    if (!msg->size) {
        return EOF;
    }

    if (raw == OFPRAW_OFPST11_GROUP_REPLY) {
        base_len = sizeof *ogs11;
        ogs11 = ofpbuf_try_pull(msg, sizeof *ogs11);
        gs->duration_sec = gs->duration_nsec = UINT32_MAX;
    } else if (raw == OFPRAW_OFPST13_GROUP_REPLY) {
        struct ofp13_group_stats *ogs13;

        base_len = sizeof *ogs13;
        ogs13 = ofpbuf_try_pull(msg, sizeof *ogs13);
        if (ogs13) {
            ogs11 = &ogs13->gs;
            gs->duration_sec = ntohl(ogs13->duration_sec);
            gs->duration_nsec = ntohl(ogs13->duration_nsec);
        } else {
            ogs11 = NULL;
        }
    } else {
        OVS_NOT_REACHED();
    }

    if (!ogs11) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "%s reply has %"PRIu32" leftover bytes at end",
                     ofpraw_get_name(raw), msg->size);
        return OFPERR_OFPBRC_BAD_LEN;
    }
    length = ntohs(ogs11->length);
    if (length < sizeof base_len) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "%s reply claims invalid length %"PRIuSIZE,
                     ofpraw_get_name(raw), length);
        return OFPERR_OFPBRC_BAD_LEN;
    }

    gs->group_id = ntohl(ogs11->group_id);
    gs->ref_count = ntohl(ogs11->ref_count);
    gs->packet_count = ntohll(ogs11->packet_count);
    gs->byte_count = ntohll(ogs11->byte_count);

    gs->n_buckets = (length - base_len) / sizeof *obc;
    obc = ofpbuf_try_pull(msg, gs->n_buckets * sizeof *obc);
    if (!obc) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "%s reply has %"PRIu32" leftover bytes at end",
                     ofpraw_get_name(raw), msg->size);
        return OFPERR_OFPBRC_BAD_LEN;
    }

    gs->bucket_stats = xmalloc(gs->n_buckets * sizeof *gs->bucket_stats);
    for (i = 0; i < gs->n_buckets; i++) {
        gs->bucket_stats[i].packet_count = ntohll(obc[i].packet_count);
        gs->bucket_stats[i].byte_count = ntohll(obc[i].byte_count);
    }

    return 0;
}
