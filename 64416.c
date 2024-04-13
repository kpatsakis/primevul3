update_mtu_ofproto(struct ofproto *p)
{
    struct ofport *ofport;
    int old_min = p->min_mtu;

    p->min_mtu = find_min_mtu(p);
    if (p->min_mtu == old_min) {
        return;
    }

    HMAP_FOR_EACH (ofport, hmap_node, &p->ports) {
        struct netdev *netdev = ofport->netdev;

        if (ofport_is_mtu_overridden(p, ofport)) {
            if (!netdev_set_mtu(netdev, p->min_mtu)) {
                ofport->mtu = p->min_mtu;
            }
        }
    }
}
