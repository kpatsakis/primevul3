update_mtu(struct ofproto *p, struct ofport *port)
{
    struct netdev *netdev = port->netdev;
    int dev_mtu;

    if (netdev_get_mtu(netdev, &dev_mtu)) {
        port->mtu = 0;
        return;
    }
    if (ofport_is_mtu_overridden(p, port)) {
        if (dev_mtu > p->min_mtu) {
            if (!netdev_set_mtu(port->netdev, p->min_mtu)) {
                dev_mtu = p->min_mtu;
            }
        }
        port->mtu = dev_mtu;
        return;
    }

    port->mtu = dev_mtu;
    /* For non-overridden port find new min mtu. */

    update_mtu_ofproto(p);
}
