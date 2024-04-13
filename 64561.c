ofputil_encode_queue_get_config_request(enum ofp_version version,
                                        ofp_port_t port,
                                        uint32_t queue)
{
    struct ofpbuf *request;

    if (version == OFP10_VERSION) {
        struct ofp10_queue_get_config_request *qgcr10;

        request = ofpraw_alloc(OFPRAW_OFPT10_QUEUE_GET_CONFIG_REQUEST,
                               version, 0);
        qgcr10 = ofpbuf_put_zeros(request, sizeof *qgcr10);
        qgcr10->port = htons(ofp_to_u16(port));
    } else if (version < OFP14_VERSION) {
        struct ofp11_queue_get_config_request *qgcr11;

        request = ofpraw_alloc(OFPRAW_OFPT11_QUEUE_GET_CONFIG_REQUEST,
                               version, 0);
        qgcr11 = ofpbuf_put_zeros(request, sizeof *qgcr11);
        qgcr11->port = ofputil_port_to_ofp11(port);
    } else {
        struct ofp14_queue_desc_request *qdr14;

        request = ofpraw_alloc(OFPRAW_OFPST14_QUEUE_DESC_REQUEST,
                               version, 0);
        qdr14 = ofpbuf_put_zeros(request, sizeof *qdr14);
        qdr14->port = ofputil_port_to_ofp11(port);
        qdr14->queue = htonl(queue);
    }

    return request;
}
