handle_nxt_set_packet_in_format(struct ofconn *ofconn,
                                const struct ofp_header *oh)
{
    const struct nx_set_packet_in_format *msg = ofpmsg_body(oh);
    uint32_t format;

    format = ntohl(msg->format);
    if (!ofputil_packet_in_format_is_valid(format)) {
        return OFPERR_OFPBRC_EPERM;
    }

    ofconn_set_packet_in_format(ofconn, format);
    return 0;
}
