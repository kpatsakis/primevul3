ofputil_decode_port_desc_stats_request(const struct ofp_header *request,
                                       ofp_port_t *port)
{
    struct ofpbuf b = ofpbuf_const_initializer(request,
                                               ntohs(request->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);
    if (raw == OFPRAW_OFPST10_PORT_DESC_REQUEST) {
        *port = OFPP_ANY;
        return 0;
    } else if (raw == OFPRAW_OFPST15_PORT_DESC_REQUEST) {
        ovs_be32 *ofp11_port;

        ofp11_port = ofpbuf_pull(&b, sizeof *ofp11_port);
        return ofputil_port_from_ofp11(*ofp11_port, port);
    } else {
        OVS_NOT_REACHED();
    }
}
