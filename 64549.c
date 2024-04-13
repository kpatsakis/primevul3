ofputil_encode_nx_set_flow_format(enum nx_flow_format nxff)
{
    struct nx_set_flow_format *sff;
    struct ofpbuf *msg;

    ovs_assert(ofputil_nx_flow_format_is_valid(nxff));

    msg = ofpraw_alloc(OFPRAW_NXT_SET_FLOW_FORMAT, OFP10_VERSION, 0);
    sff = ofpbuf_put_zeros(msg, sizeof *sff);
    sff->format = htonl(nxff);

    return msg;
}
