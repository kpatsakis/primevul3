ofport_modified(struct ofport *port, struct ofputil_phy_port *pp)
{
    port->pp.hw_addr = pp->hw_addr;
    port->pp.config = ((port->pp.config & ~OFPUTIL_PC_PORT_DOWN)
                        | (pp->config & OFPUTIL_PC_PORT_DOWN));
    port->pp.state = ((port->pp.state & ~OFPUTIL_PS_LINK_DOWN)
                      | (pp->state & OFPUTIL_PS_LINK_DOWN));
    port->pp.curr = pp->curr;
    port->pp.advertised = pp->advertised;
    port->pp.supported = pp->supported;
    port->pp.peer = pp->peer;
    port->pp.curr_speed = pp->curr_speed;
    port->pp.max_speed = pp->max_speed;

    connmgr_send_port_status(port->ofproto->connmgr, NULL,
                             &port->pp, OFPPR_MODIFY);
}
