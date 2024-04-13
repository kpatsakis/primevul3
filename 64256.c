handle_port_stats_request(struct ofconn *ofconn,
                          const struct ofp_header *request)
{
    ofp_port_t port_no;
    enum ofperr error;

    error = ofputil_decode_port_stats_request(request, &port_no);
    if (!error) {
        handle_port_request(ofconn, request, port_no, append_port_stat);
    }
    return error;
}
