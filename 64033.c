ofpacts_check_consistency(struct ofpact ofpacts[], size_t ofpacts_len,
                          struct flow *flow, ofp_port_t max_ports,
                          uint8_t table_id, uint8_t n_tables,
                          enum ofputil_protocol usable_protocols)
{
    enum ofputil_protocol p = usable_protocols;
    enum ofperr error;

    error = ofpacts_check(ofpacts, ofpacts_len, flow, max_ports,
                          table_id, n_tables, &p);
    return (error ? error
            : p != usable_protocols ? OFPERR_OFPBAC_MATCH_INCONSISTENT
            : 0);
}
