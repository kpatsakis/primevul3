ofputil_decode_queue_get_config_request(const struct ofp_header *oh,
                                        ofp_port_t *port, uint32_t *queue)
{
    const struct ofp10_queue_get_config_request *qgcr10;
    const struct ofp11_queue_get_config_request *qgcr11;
    const struct ofp14_queue_desc_request *qdr14;
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);

    switch ((int) raw) {
    case OFPRAW_OFPT10_QUEUE_GET_CONFIG_REQUEST:
        qgcr10 = b.data;
        *port = u16_to_ofp(ntohs(qgcr10->port));
        *queue = OFPQ_ALL;
        break;

    case OFPRAW_OFPT11_QUEUE_GET_CONFIG_REQUEST:
        qgcr11 = b.data;
        *queue = OFPQ_ALL;
        enum ofperr error = ofputil_port_from_ofp11(qgcr11->port, port);
        if (error || *port == OFPP_ANY) {
            return error;
        }
        break;

    case OFPRAW_OFPST14_QUEUE_DESC_REQUEST:
        qdr14 = b.data;
        *queue = ntohl(qdr14->queue);
        return ofputil_port_from_ofp11(qdr14->port, port);

    default:
        OVS_NOT_REACHED();
    }

    return (ofp_to_u16(*port) < ofp_to_u16(OFPP_MAX)
            ? 0
            : OFPERR_OFPQOFC_BAD_PORT);
}
