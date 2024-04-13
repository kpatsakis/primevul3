handle_nxt_ct_flush_zone(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    const struct nx_zone_id *nzi = ofpmsg_body(oh);

    if (!is_all_zeros(nzi->zero, sizeof nzi->zero)) {
        return OFPERR_NXBRC_MUST_BE_ZERO;
    }

    uint16_t zone = ntohs(nzi->zone_id);
    if (ofproto->ofproto_class->ct_flush) {
        ofproto->ofproto_class->ct_flush(ofproto, &zone);
    } else {
        return EOPNOTSUPP;
    }

    return 0;
}
