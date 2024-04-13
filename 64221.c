dealloc_ofp_port(struct ofproto *ofproto, ofp_port_t ofp_port)
{
    if (ofp_to_u16(ofp_port) < ofproto->max_ports) {
        ofport_set_usage(ofproto, ofp_port, time_msec());
    }
}
