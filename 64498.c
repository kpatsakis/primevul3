ofputil_decode_ofpst11_flow_request(struct ofputil_flow_stats_request *fsr,
                                    struct ofpbuf *b, bool aggregate,
                                    const struct tun_table *tun_table,
                                    const struct vl_mff_map *vl_mff_map)
{
    const struct ofp11_flow_stats_request *ofsr;
    enum ofperr error;

    ofsr = ofpbuf_pull(b, sizeof *ofsr);
    fsr->aggregate = aggregate;
    fsr->table_id = ofsr->table_id;
    error = ofputil_port_from_ofp11(ofsr->out_port, &fsr->out_port);
    if (error) {
        return error;
    }
    fsr->out_group = ntohl(ofsr->out_group);
    fsr->cookie = ofsr->cookie;
    fsr->cookie_mask = ofsr->cookie_mask;
    error = ofputil_pull_ofp11_match(b, tun_table, vl_mff_map, &fsr->match,
                                     NULL);
    if (error) {
        return error;
    }

    return 0;
}
