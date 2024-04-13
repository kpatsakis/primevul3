port_mod_finish(struct ofconn *ofconn, struct ofputil_port_mod *pm,
                struct ofport *port)
{
    update_port_config(ofconn, port, pm->config, pm->mask);
    if (pm->advertise) {
        netdev_set_advertisements(port->netdev, pm->advertise);
    }
}
