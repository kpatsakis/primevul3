ofproto_port_destroy(struct ofproto_port *ofproto_port)
{
    free(ofproto_port->name);
    free(ofproto_port->type);
}
