ofproto_port_set_state(struct ofport *port, enum ofputil_port_state state)
{
    if (port->pp.state != state) {
        port->pp.state = state;
        connmgr_send_port_status(port->ofproto->connmgr, NULL,
                                 &port->pp, OFPPR_MODIFY);
    }
}
