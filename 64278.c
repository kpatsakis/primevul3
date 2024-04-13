ofport_destroy(struct ofport *port, bool del)
{
    if (port) {
        dealloc_ofp_port(port->ofproto, port->ofp_port);
        port->ofproto->ofproto_class->port_destruct(port, del);
        ofport_destroy__(port);
     }
}
