ofproto_port_set_bfd(struct ofproto *ofproto, ofp_port_t ofp_port,
                     const struct smap *cfg)
{
    struct ofport *ofport;
    int error;

    ofport = ofproto_get_port(ofproto, ofp_port);
    if (!ofport) {
        VLOG_WARN("%s: cannot configure bfd on nonexistent port %"PRIu32,
                  ofproto->name, ofp_port);
        return;
    }

    error = (ofproto->ofproto_class->set_bfd
             ? ofproto->ofproto_class->set_bfd(ofport, cfg)
             : EOPNOTSUPP);
    if (error) {
        VLOG_WARN("%s: bfd configuration on port %"PRIu32" (%s) failed (%s)",
                  ofproto->name, ofp_port, netdev_get_name(ofport->netdev),
                  ovs_strerror(error));
    }
}
