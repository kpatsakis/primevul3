ofproto_port_get_stp_stats(struct ofproto *ofproto, ofp_port_t ofp_port,
                           struct ofproto_port_stp_stats *s)
{
    struct ofport *ofport = ofproto_get_port(ofproto, ofp_port);
    if (!ofport) {
        VLOG_WARN_RL(&rl, "%s: cannot get STP stats on nonexistent "
                     "port %"PRIu32, ofproto->name, ofp_port);
        return ENODEV;
    }

    return (ofproto->ofproto_class->get_stp_port_stats
            ? ofproto->ofproto_class->get_stp_port_stats(ofport, s)
            : EOPNOTSUPP);
}
