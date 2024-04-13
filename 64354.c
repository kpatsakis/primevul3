ofproto_port_set_lldp(struct ofproto *ofproto,
                      ofp_port_t ofp_port,
                      const struct smap *cfg)
{
    struct ofport *ofport;
    int error;

    ofport = ofproto_get_port(ofproto, ofp_port);
    if (!ofport) {
        VLOG_WARN("%s: cannot configure LLDP on nonexistent port %"PRIu32,
                  ofproto->name, ofp_port);
        return;
    }
    error = (ofproto->ofproto_class->set_lldp
             ? ofproto->ofproto_class->set_lldp(ofport, cfg)
             : EOPNOTSUPP);
    if (error) {
        VLOG_WARN("%s: lldp configuration on port %"PRIu32" (%s) failed (%s)",
                  ofproto->name, ofp_port, netdev_get_name(ofport->netdev),
                  ovs_strerror(error));
    }
}
