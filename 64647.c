ofputil_pull_queue_get_config_reply10(struct ofpbuf *msg,
                                      struct ofputil_queue_config *queue)
{
    const struct ofp_header *oh = msg->header;
    unsigned int opq_len;       /* Length of protocol-specific queue header. */
    unsigned int len;           /* Total length of queue + properties. */

    /* Obtain the port number from the message header. */
    if (oh->version == OFP10_VERSION) {
        const struct ofp10_queue_get_config_reply *oqgcr10 = msg->msg;
        queue->port = u16_to_ofp(ntohs(oqgcr10->port));
    } else {
        const struct ofp11_queue_get_config_reply *oqgcr11 = msg->msg;
        enum ofperr error = ofputil_port_from_ofp11(oqgcr11->port,
                                                    &queue->port);
        if (error) {
            return error;
        }
    }

    /* Pull off the queue header and get the queue number and length. */
    if (oh->version < OFP12_VERSION) {
        const struct ofp10_packet_queue *opq10;
        opq10 = ofpbuf_try_pull(msg, sizeof *opq10);
        if (!opq10) {
            return OFPERR_OFPBRC_BAD_LEN;
        }
        queue->queue = ntohl(opq10->queue_id);
        len = ntohs(opq10->len);
        opq_len = sizeof *opq10;
    } else {
        const struct ofp12_packet_queue *opq12;
        opq12 = ofpbuf_try_pull(msg, sizeof *opq12);
        if (!opq12) {
            return OFPERR_OFPBRC_BAD_LEN;
        }
        queue->queue = ntohl(opq12->queue_id);
        len = ntohs(opq12->len);
        opq_len = sizeof *opq12;
    }

    /* Length check. */
    if (len < opq_len || len > msg->size + opq_len || len % 8) {
        return OFPERR_OFPBRC_BAD_LEN;
    }
    len -= opq_len;

    /* Pull properties.  The format of these properties differs from used in
     * OF1.4+ so we can't use the common property functions. */
    while (len > 0) {
        const struct ofp10_queue_prop_header *hdr;
        unsigned int property;
        unsigned int prop_len;
        enum ofperr error = 0;

        hdr = ofpbuf_at_assert(msg, 0, sizeof *hdr);
        prop_len = ntohs(hdr->len);
        if (prop_len < sizeof *hdr || prop_len > len || prop_len % 8) {
            return OFPERR_OFPBRC_BAD_LEN;
        }

        property = ntohs(hdr->property);
        switch (property) {
        case OFPQT10_MIN_RATE:
            error = parse_ofp10_queue_rate(hdr, &queue->min_rate);
            break;

        case OFPQT11_MAX_RATE:
            error = parse_ofp10_queue_rate(hdr, &queue->max_rate);
            break;

        default:
            VLOG_INFO_RL(&bad_ofmsg_rl, "unknown queue property %u", property);
            break;
        }
        if (error) {
            return error;
        }

        ofpbuf_pull(msg, prop_len);
        len -= prop_len;
    }
    return 0;
}
