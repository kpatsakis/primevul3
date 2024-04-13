ofputil_decode_flow_stats_reply(struct ofputil_flow_stats *fs,
                                struct ofpbuf *msg,
                                bool flow_age_extension,
                                struct ofpbuf *ofpacts)
{
    const struct ofp_header *oh;
    size_t instructions_len;
    enum ofperr error;
    enum ofpraw raw;

    error = (msg->header ? ofpraw_decode(&raw, msg->header)
             : ofpraw_pull(&raw, msg));
    if (error) {
        return error;
    }
    oh = msg->header;

    if (!msg->size) {
        return EOF;
    } else if (raw == OFPRAW_OFPST11_FLOW_REPLY
               || raw == OFPRAW_OFPST13_FLOW_REPLY) {
        const struct ofp11_flow_stats *ofs;
        size_t length;
        uint16_t padded_match_len;

        ofs = ofpbuf_try_pull(msg, sizeof *ofs);
        if (!ofs) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "OFPST_FLOW reply has %"PRIu32" leftover "
                         "bytes at end", msg->size);
            return EINVAL;
        }

        length = ntohs(ofs->length);
        if (length < sizeof *ofs) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "OFPST_FLOW reply claims invalid "
                         "length %"PRIuSIZE, length);
            return EINVAL;
        }

        if (ofputil_pull_ofp11_match(msg, NULL, NULL, &fs->match,
                                     &padded_match_len)) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "OFPST_FLOW reply bad match");
            return EINVAL;
        }
        instructions_len = length - sizeof *ofs - padded_match_len;

        fs->priority = ntohs(ofs->priority);
        fs->table_id = ofs->table_id;
        fs->duration_sec = ntohl(ofs->duration_sec);
        fs->duration_nsec = ntohl(ofs->duration_nsec);
        fs->idle_timeout = ntohs(ofs->idle_timeout);
        fs->hard_timeout = ntohs(ofs->hard_timeout);
        if (oh->version >= OFP14_VERSION) {
            fs->importance = ntohs(ofs->importance);
        } else {
            fs->importance = 0;
        }
        if (raw == OFPRAW_OFPST13_FLOW_REPLY) {
            error = ofputil_decode_flow_mod_flags(ofs->flags, -1, oh->version,
                                                  &fs->flags);
            if (error) {
                return error;
            }
        } else {
            fs->flags = 0;
        }
        fs->idle_age = -1;
        fs->hard_age = -1;
        fs->cookie = ofs->cookie;
        fs->packet_count = ntohll(ofs->packet_count);
        fs->byte_count = ntohll(ofs->byte_count);
    } else if (raw == OFPRAW_OFPST10_FLOW_REPLY) {
        const struct ofp10_flow_stats *ofs;
        size_t length;

        ofs = ofpbuf_try_pull(msg, sizeof *ofs);
        if (!ofs) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "OFPST_FLOW reply has %"PRIu32" leftover "
                         "bytes at end", msg->size);
            return EINVAL;
        }

        length = ntohs(ofs->length);
        if (length < sizeof *ofs) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "OFPST_FLOW reply claims invalid "
                         "length %"PRIuSIZE, length);
            return EINVAL;
        }
        instructions_len = length - sizeof *ofs;

        fs->cookie = get_32aligned_be64(&ofs->cookie);
        ofputil_match_from_ofp10_match(&ofs->match, &fs->match);
        fs->priority = ntohs(ofs->priority);
        fs->table_id = ofs->table_id;
        fs->duration_sec = ntohl(ofs->duration_sec);
        fs->duration_nsec = ntohl(ofs->duration_nsec);
        fs->idle_timeout = ntohs(ofs->idle_timeout);
        fs->hard_timeout = ntohs(ofs->hard_timeout);
        fs->importance = 0;
        fs->idle_age = -1;
        fs->hard_age = -1;
        fs->packet_count = ntohll(get_32aligned_be64(&ofs->packet_count));
        fs->byte_count = ntohll(get_32aligned_be64(&ofs->byte_count));
        fs->flags = 0;
    } else if (raw == OFPRAW_NXST_FLOW_REPLY) {
        const struct nx_flow_stats *nfs;
        size_t match_len, length;

        nfs = ofpbuf_try_pull(msg, sizeof *nfs);
        if (!nfs) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "NXST_FLOW reply has %"PRIu32" leftover "
                         "bytes at end", msg->size);
            return EINVAL;
        }

        length = ntohs(nfs->length);
        match_len = ntohs(nfs->match_len);
        if (length < sizeof *nfs + ROUND_UP(match_len, 8)) {
            VLOG_WARN_RL(&bad_ofmsg_rl, "NXST_FLOW reply with match_len=%"PRIuSIZE" "
                         "claims invalid length %"PRIuSIZE, match_len, length);
            return EINVAL;
        }
        if (nx_pull_match(msg, match_len, &fs->match, NULL, NULL, NULL,
                          NULL)) {
            return EINVAL;
        }
        instructions_len = length - sizeof *nfs - ROUND_UP(match_len, 8);

        fs->cookie = nfs->cookie;
        fs->table_id = nfs->table_id;
        fs->duration_sec = ntohl(nfs->duration_sec);
        fs->duration_nsec = ntohl(nfs->duration_nsec);
        fs->priority = ntohs(nfs->priority);
        fs->idle_timeout = ntohs(nfs->idle_timeout);
        fs->hard_timeout = ntohs(nfs->hard_timeout);
        fs->importance = 0;
        fs->idle_age = -1;
        fs->hard_age = -1;
        if (flow_age_extension) {
            if (nfs->idle_age) {
                fs->idle_age = ntohs(nfs->idle_age) - 1;
            }
            if (nfs->hard_age) {
                fs->hard_age = ntohs(nfs->hard_age) - 1;
            }
        }
        fs->packet_count = ntohll(nfs->packet_count);
        fs->byte_count = ntohll(nfs->byte_count);
        fs->flags = 0;
    } else {
        OVS_NOT_REACHED();
    }

    if (ofpacts_pull_openflow_instructions(msg, instructions_len, oh->version,
                                           NULL, NULL, ofpacts)) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "OFPST_FLOW reply bad instructions");
        return EINVAL;
    }
    fs->ofpacts = ofpacts->data;
    fs->ofpacts_len = ofpacts->size;

    return 0;
}
