ofputil_encode_flow_monitor_cancel(uint32_t id)
{
    struct nx_flow_monitor_cancel *nfmc;
    struct ofpbuf *msg;

    msg = ofpraw_alloc(OFPRAW_NXT_FLOW_MONITOR_CANCEL, OFP10_VERSION, 0);
    nfmc = ofpbuf_put_uninit(msg, sizeof *nfmc);
    nfmc->id = htonl(id);
    return msg;
}
