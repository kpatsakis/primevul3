ofpacts_check(struct ofpact ofpacts[], size_t ofpacts_len,
              struct flow *flow, ofp_port_t max_ports,
              uint8_t table_id, uint8_t n_tables,
              enum ofputil_protocol *usable_protocols)
{
    struct ofpact *a;
    ovs_be16 dl_type = flow->dl_type;
    ovs_be16 vlan_tci = flow->vlan_tci;
    uint8_t nw_proto = flow->nw_proto;
    enum ofperr error = 0;

    OFPACT_FOR_EACH (a, ofpacts, ofpacts_len) {
        error = ofpact_check__(usable_protocols, a, flow,
                               max_ports, table_id, n_tables);
        if (error) {
            break;
        }
    }
    /* Restore fields that may have been modified. */
    flow->dl_type = dl_type;
    flow->vlan_tci = vlan_tci;
    flow->nw_proto = nw_proto;
    return error;
}
