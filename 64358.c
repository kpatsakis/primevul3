ofproto_port_set_stp(struct ofproto *ofproto, ofp_port_t ofp_port,
                     const struct ofproto_port_stp_settings *s)
{
    struct ofport *ofport = ofproto_get_port(ofproto, ofp_port);
    if (!ofport) {
        VLOG_WARN("%s: cannot configure STP on nonexistent port %"PRIu32,
                  ofproto->name, ofp_port);
        return ENODEV;
    }

    return (ofproto->ofproto_class->set_stp_port
            ? ofproto->ofproto_class->set_stp_port(ofport, s)
            : EOPNOTSUPP);
}
