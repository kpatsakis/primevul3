port_mod_start(struct ofconn *ofconn, struct ofputil_port_mod *pm,
               struct ofport **port)
{
    struct ofproto *p = ofconn_get_ofproto(ofconn);

    *port = ofproto_get_port(p, pm->port_no);
    if (!*port) {
        return OFPERR_OFPPMFC_BAD_PORT;
    }
    if (!eth_addr_equals((*port)->pp.hw_addr, pm->hw_addr)) {
        return OFPERR_OFPPMFC_BAD_HW_ADDR;
    }
    return 0;
}
