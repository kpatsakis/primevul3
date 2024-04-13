ofputil_pull_queue_get_config_reply14(struct ofpbuf *msg,
                                      struct ofputil_queue_config *queue)
{
    struct ofp14_queue_desc *oqd14 = ofpbuf_try_pull(msg, sizeof *oqd14);
    if (!oqd14) {
        return OFPERR_OFPBRC_BAD_LEN;
    }
    enum ofperr error = ofputil_port_from_ofp11(oqd14->port_no, &queue->port);
    if (error) {
        return error;
    }
    queue->queue = ntohl(oqd14->queue_id);

    /* Length check. */
    unsigned int len = ntohs(oqd14->len);
    if (len < sizeof *oqd14 || len > msg->size + sizeof *oqd14 || len % 8) {
        return OFPERR_OFPBRC_BAD_LEN;
    }
    len -= sizeof *oqd14;

    struct ofpbuf properties = ofpbuf_const_initializer(ofpbuf_pull(msg, len),
                                                        len);
    while (properties.size > 0) {
        struct ofpbuf payload;
        uint64_t type;

        error = ofpprop_pull(&properties, &payload, &type);
        if (error) {
            return error;
        }

        switch (type) {
        case OFPQDPT14_MIN_RATE:
            error = ofpprop_parse_u16(&payload, &queue->min_rate);
            break;

        case OFPQDPT14_MAX_RATE:
            error = ofpprop_parse_u16(&payload, &queue->max_rate);
            break;

        default:
            error = OFPPROP_UNKNOWN(true, "queue desc", type);
            break;
        }

        if (error) {
            return error;
        }
    }

    return 0;
}
