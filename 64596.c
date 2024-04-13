ofputil_make_set_packet_in_format(enum ofp_version ofp_version,
                                  enum nx_packet_in_format packet_in_format)
{
    struct nx_set_packet_in_format *spif;
    struct ofpbuf *msg;

    msg = ofpraw_alloc(OFPRAW_NXT_SET_PACKET_IN_FORMAT, ofp_version, 0);
    spif = ofpbuf_put_zeros(msg, sizeof *spif);
    spif->format = htonl(packet_in_format);

    return msg;
}
