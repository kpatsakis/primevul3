ofproto_port_clone(struct ofproto_port *port, const struct ofproto_port *old)
{
    port->name = xstrdup(old->name);
    port->type = xstrdup(old->type);
    port->ofp_port = old->ofp_port;
}
