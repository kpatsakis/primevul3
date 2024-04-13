ofputil_decode_nxst_flow_request(struct ofputil_flow_stats_request *fsr,
                                 struct ofpbuf *b, bool aggregate,
                                 const struct tun_table *tun_table,
                                 const struct vl_mff_map *vl_mff_map)
{
    const struct nx_flow_stats_request *nfsr;
    enum ofperr error;

    nfsr = ofpbuf_pull(b, sizeof *nfsr);
    error = nx_pull_match(b, ntohs(nfsr->match_len), &fsr->match,
                          &fsr->cookie, &fsr->cookie_mask, tun_table,
                          vl_mff_map);
    if (error) {
        return error;
    }
    if (b->size) {
        return OFPERR_OFPBRC_BAD_LEN;
    }

    fsr->aggregate = aggregate;
    fsr->out_port = u16_to_ofp(ntohs(nfsr->out_port));
    fsr->out_group = OFPG_ANY;
    fsr->table_id = nfsr->table_id;

    return 0;
}
