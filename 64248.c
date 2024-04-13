handle_nxt_resume(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_packet_in_private pin;
    enum ofperr error;

    error = ofputil_decode_packet_in_private(oh, false,
                                             ofproto_get_tun_tab(ofproto),
                                             &ofproto->vl_mff_map, &pin, NULL,
                                             NULL);
    if (error) {
        return error;
    }

    error = (ofproto->ofproto_class->nxt_resume
             ? ofproto->ofproto_class->nxt_resume(ofproto, &pin)
             : OFPERR_NXR_NOT_SUPPORTED);

    ofputil_packet_in_private_destroy(&pin);

    return error;
}
