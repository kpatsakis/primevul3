ofproto_port_bfd_status_changed(struct ofproto *ofproto, ofp_port_t ofp_port)
{
    struct ofport *ofport = ofproto_get_port(ofproto, ofp_port);
    return (ofport && ofproto->ofproto_class->bfd_status_changed
            ? ofproto->ofproto_class->bfd_status_changed(ofport)
            : true);
}
