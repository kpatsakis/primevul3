ofputil_pull_ipfix_stats(struct ofputil_ipfix_stats *is, struct ofpbuf *msg)
{
    enum ofperr error;
    enum ofpraw raw;

    memset(is, 0xFF, sizeof (*is));

    error = (msg->header ? ofpraw_decode(&raw, msg->header)
             : ofpraw_pull(&raw, msg));
    if (error) {
        return error;
    }

    if (!msg->size) {
        return EOF;
    } else if (raw == OFPRAW_NXST_IPFIX_BRIDGE_REPLY ||
               raw == OFPRAW_NXST_IPFIX_FLOW_REPLY) {
        struct nx_ipfix_stats_reply *reply;

        reply = ofpbuf_try_pull(msg, sizeof *reply);
        return ofputil_ipfix_stats_from_nx(is, reply);
    } else {
        OVS_NOT_REACHED();
    }
}
