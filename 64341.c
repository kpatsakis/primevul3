ofproto_port_get_bfd_status(struct ofproto *ofproto, ofp_port_t ofp_port,
                            struct smap *status)
{
    struct ofport *ofport = ofproto_get_port(ofproto, ofp_port);
    return (ofport && ofproto->ofproto_class->get_bfd_status
            ? ofproto->ofproto_class->get_bfd_status(ofport, status)
            : EOPNOTSUPP);
}
