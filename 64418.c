update_port_config(struct ofconn *ofconn, struct ofport *port,
                   enum ofputil_port_config config,
                   enum ofputil_port_config mask)
{
    enum ofputil_port_config toggle = (config ^ port->pp.config) & mask;

    if (toggle & OFPUTIL_PC_PORT_DOWN
        && (config & OFPUTIL_PC_PORT_DOWN
            ? netdev_turn_flags_off(port->netdev, NETDEV_UP, NULL)
            : netdev_turn_flags_on(port->netdev, NETDEV_UP, NULL))) {
        /* We tried to bring the port up or down, but it failed, so don't
         * update the "down" bit. */
        toggle &= ~OFPUTIL_PC_PORT_DOWN;
    }

    if (toggle) {
        enum ofputil_port_config old_config = port->pp.config;
        port->pp.config ^= toggle;
        port->ofproto->ofproto_class->port_reconfigured(port, old_config);
        connmgr_send_port_status(port->ofproto->connmgr, ofconn, &port->pp,
                                 OFPPR_MODIFY);
    }
}
