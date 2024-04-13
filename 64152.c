    OVS_EXCLUDED(ofproto_mutex)
{
    enum ofperr error = handle_openflow__(ofconn, ofp_msg);

    if (error) {
        ofconn_send_error(ofconn, ofp_msg->data, error);
    }
    COVERAGE_INC(ofproto_recv_openflow);
}
