find_min_mtu(struct ofproto *p)
{
    struct ofport *ofport;
    int mtu = 0;

    HMAP_FOR_EACH (ofport, hmap_node, &p->ports) {
        struct netdev *netdev = ofport->netdev;
        int dev_mtu;

        /* Skip any overridden port, since that's what we're trying to set. */
        if (ofport_is_mtu_overridden(p, ofport)) {
            continue;
        }

        if (netdev_get_mtu(netdev, &dev_mtu)) {
            continue;
        }
        if (!mtu || dev_mtu < mtu) {
            mtu = dev_mtu;
        }
    }

    return mtu ? mtu: ETH_PAYLOAD_MAX;
}
