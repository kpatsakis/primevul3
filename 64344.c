ofproto_port_get_rstp_status(struct ofproto *ofproto, ofp_port_t ofp_port,
                             struct ofproto_port_rstp_status *s)
{
    struct ofport *ofport = ofproto_get_port(ofproto, ofp_port);
    if (!ofport) {
        VLOG_WARN_RL(&rl, "%s: cannot get RSTP status on nonexistent "
                "port %"PRIu32, ofproto->name, ofp_port);
        return ENODEV;
    }

    if (!ofproto->ofproto_class->get_rstp_port_status) {
        return  EOPNOTSUPP;
    }
    ofproto->ofproto_class->get_rstp_port_status(ofport, s);
    return 0;
}
