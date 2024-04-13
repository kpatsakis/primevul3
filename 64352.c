ofproto_port_set_cfm(struct ofproto *ofproto, ofp_port_t ofp_port,
                     const struct cfm_settings *s)
{
    struct ofport *ofport;
    int error;

    ofport = ofproto_get_port(ofproto, ofp_port);
    if (!ofport) {
        VLOG_WARN("%s: cannot configure CFM on nonexistent port %"PRIu32,
                  ofproto->name, ofp_port);
        return;
    }

    /* XXX: For configuration simplicity, we only support one remote_mpid
     * outside of the CFM module.  It's not clear if this is the correct long
     * term solution or not. */
    error = (ofproto->ofproto_class->set_cfm
             ? ofproto->ofproto_class->set_cfm(ofport, s)
             : EOPNOTSUPP);
    if (error) {
        VLOG_WARN("%s: CFM configuration on port %"PRIu32" (%s) failed (%s)",
                  ofproto->name, ofp_port, netdev_get_name(ofport->netdev),
                  ovs_strerror(error));
    }
}
