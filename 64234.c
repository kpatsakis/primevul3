handle_desc_stats_request(struct ofconn *ofconn,
                          const struct ofp_header *request)
{
    static const char *default_mfr_desc = "Nicira, Inc.";
    static const char *default_hw_desc = "Open vSwitch";
    static const char *default_sw_desc = VERSION;
    static const char *default_serial_desc = "None";
    static const char *default_dp_desc = "None";

    struct ofproto *p = ofconn_get_ofproto(ofconn);
    struct ofp_desc_stats *ods;
    struct ofpbuf *msg;

    msg = ofpraw_alloc_stats_reply(request, 0);
    ods = ofpbuf_put_zeros(msg, sizeof *ods);
    ovs_strlcpy(ods->mfr_desc, p->mfr_desc ? p->mfr_desc : default_mfr_desc,
                sizeof ods->mfr_desc);
    ovs_strlcpy(ods->hw_desc, p->hw_desc ? p->hw_desc : default_hw_desc,
                sizeof ods->hw_desc);
    ovs_strlcpy(ods->sw_desc, p->sw_desc ? p->sw_desc : default_sw_desc,
                sizeof ods->sw_desc);
    ovs_strlcpy(ods->serial_num,
                p->serial_desc ? p->serial_desc : default_serial_desc,
                sizeof ods->serial_num);
    ovs_strlcpy(ods->dp_desc, p->dp_desc ? p->dp_desc : default_dp_desc,
                sizeof ods->dp_desc);
    ofconn_send_reply(ofconn, msg);

    return 0;
}
