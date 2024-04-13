ofproto_get_port(const struct ofproto *ofproto, ofp_port_t ofp_port)
{
    struct ofport *port;

    HMAP_FOR_EACH_IN_BUCKET (port, hmap_node, hash_ofp_port(ofp_port),
                             &ofproto->ports) {
        if (port->ofp_port == ofp_port) {
            return port;
        }
    }
    return NULL;
}
