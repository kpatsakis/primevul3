ofproto_port_get_cfm_status(const struct ofproto *ofproto, ofp_port_t ofp_port,
                            struct cfm_status *status)
{
    struct ofport *ofport = ofproto_get_port(ofproto, ofp_port);
    return (ofport && ofproto->ofproto_class->get_cfm_status
            ? ofproto->ofproto_class->get_cfm_status(ofport, status)
            : EOPNOTSUPP);
}
