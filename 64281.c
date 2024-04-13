ofport_install(struct ofproto *p,
               struct netdev *netdev, const struct ofputil_phy_port *pp)
{
    const char *netdev_name = netdev_get_name(netdev);
    struct ofport *ofport;
    int error;

    /* Create ofport. */
    ofport = p->ofproto_class->port_alloc();
    if (!ofport) {
        error = ENOMEM;
        goto error;
    }
    ofport->ofproto = p;
    ofport->netdev = netdev;
    ofport->change_seq = netdev_get_change_seq(netdev);
    ofport->pp = *pp;
    ofport->ofp_port = pp->port_no;
    ofport->created = time_msec();

    /* Add port to 'p'. */
    hmap_insert(&p->ports, &ofport->hmap_node,
                hash_ofp_port(ofport->ofp_port));
    shash_add(&p->port_by_name, netdev_name, ofport);

    update_mtu(p, ofport);

    /* Let the ofproto_class initialize its private data. */
    error = p->ofproto_class->port_construct(ofport);
    if (error) {
        goto error;
    }
    connmgr_send_port_status(p->connmgr, NULL, pp, OFPPR_ADD);
    return 0;

error:
    VLOG_WARN_RL(&rl, "%s: could not add port %s (%s)",
                 p->name, netdev_name, ovs_strerror(error));
    if (ofport) {
        ofport_destroy__(ofport);
    } else {
        netdev_close(netdev);
    }
    return error;
}
