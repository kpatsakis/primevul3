alloc_ofp_port(struct ofproto *ofproto, const char *netdev_name)
{
    uint16_t port_idx;

    port_idx = simap_get(&ofproto->ofp_requests, netdev_name);
    port_idx = port_idx ? port_idx : UINT16_MAX;

    if (port_idx >= ofproto->max_ports
        || ofport_get_usage(ofproto, u16_to_ofp(port_idx)) == LLONG_MAX) {
        uint16_t lru_ofport = 0, end_port_no = ofproto->alloc_port_no;
        long long int last_used_at, lru = LLONG_MAX;

        /* Search for a free OpenFlow port number.  We try not to
         * immediately reuse them to prevent problems due to old
         * flows.
         *
         * We limit the automatically assigned port numbers to the lower half
         * of the port range, to reserve the upper half for assignment by
         * controllers. */
        for (;;) {
            if (++ofproto->alloc_port_no >= MIN(ofproto->max_ports, 32768)) {
                ofproto->alloc_port_no = 1;
            }
            last_used_at = ofport_get_usage(ofproto,
                                         u16_to_ofp(ofproto->alloc_port_no));
            if (!last_used_at) {
                port_idx = ofproto->alloc_port_no;
                break;
            } else if ( last_used_at < time_msec() - 60*60*1000) {
                /* If the port with ofport 'ofproto->alloc_port_no' was deleted
                 * more than an hour ago, consider it usable. */
                ofport_remove_usage(ofproto,
                    u16_to_ofp(ofproto->alloc_port_no));
                port_idx = ofproto->alloc_port_no;
                break;
            } else if (last_used_at < lru) {
                lru = last_used_at;
                lru_ofport = ofproto->alloc_port_no;
            }

            if (ofproto->alloc_port_no == end_port_no) {
                if (lru_ofport) {
                    port_idx = lru_ofport;
                    break;
                }
                return OFPP_NONE;
            }
        }
    }
    ofport_set_usage(ofproto, u16_to_ofp(port_idx), LLONG_MAX);
    return u16_to_ofp(port_idx);
}
