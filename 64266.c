handle_table_mod(struct ofconn *ofconn, const struct ofp_header *oh)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    struct ofputil_table_mod tm;
    enum ofperr error;

    error = reject_slave_controller(ofconn);
    if (error) {
        return error;
    }

    error = ofputil_decode_table_mod(oh, &tm);
    if (error) {
        return error;
    }

    return table_mod(ofproto, &tm);
}
