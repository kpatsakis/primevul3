handle_tlv_table_request(struct ofconn *ofconn, const struct ofp_header *oh)
{
    const struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_tlv_table_reply ttr;
    struct ofpbuf *b;

    tun_metadata_table_request(ofproto_get_tun_tab(ofproto), &ttr);

    b = ofputil_encode_tlv_table_reply(oh, &ttr);
    ofputil_uninit_tlv_table(&ttr.mappings);

    ofconn_send_reply(ofconn, b);
    return 0;
}
