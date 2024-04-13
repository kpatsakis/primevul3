ofproto_init_max_ports(struct ofproto *ofproto, uint16_t max_ports)
{
    ovs_assert(max_ports <= ofp_to_u16(OFPP_MAX));
    ofproto->max_ports = max_ports;
}
