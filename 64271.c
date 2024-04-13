init_ports(struct ofproto *p)
{
    struct ofproto_port_dump dump;
    struct ofproto_port ofproto_port;
    struct shash_node *node, *next;

    OFPROTO_PORT_FOR_EACH (&ofproto_port, &dump, p) {
        const char *name = ofproto_port.name;

        if (shash_find(&p->port_by_name, name)) {
            VLOG_WARN_RL(&rl, "%s: ignoring duplicate device %s in datapath",
                         p->name, name);
        } else {
            struct ofputil_phy_port pp;
            struct netdev *netdev;

            /* Check if an OpenFlow port number had been requested. */
            node = shash_find(&init_ofp_ports, name);
            if (node) {
                const struct iface_hint *iface_hint = node->data;
                simap_put(&p->ofp_requests, name,
                          ofp_to_u16(iface_hint->ofp_port));
            }

            netdev = ofport_open(p, &ofproto_port, &pp);
            if (netdev) {
                ofport_install(p, netdev, &pp);
                if (ofp_to_u16(ofproto_port.ofp_port) < p->max_ports) {
                    p->alloc_port_no = MAX(p->alloc_port_no,
                                           ofp_to_u16(ofproto_port.ofp_port));
                }
            }
        }
    }

    SHASH_FOR_EACH_SAFE(node, next, &init_ofp_ports) {
        struct iface_hint *iface_hint = node->data;

        if (!strcmp(iface_hint->br_name, p->name)) {
            free(iface_hint->br_name);
            free(iface_hint->br_type);
            free(iface_hint);
            shash_delete(&init_ofp_ports, node);
        }
    }

    return 0;
}
