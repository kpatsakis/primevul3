ofproto_packet_out_init(struct ofproto *ofproto,
                        struct ofconn *ofconn,
                        struct ofproto_packet_out *opo,
                        const struct ofputil_packet_out *po)
{
    enum ofperr error;

    if (ofp_to_u16(po->in_port) >= ofproto->max_ports
        && ofp_to_u16(po->in_port) < ofp_to_u16(OFPP_MAX)) {
        return OFPERR_OFPBRC_BAD_PORT;
    }

    /* Get payload. */
    if (po->buffer_id != UINT32_MAX) {
        return OFPERR_OFPBRC_BUFFER_UNKNOWN;
    }

    /* Ensure that the L3 header is 32-bit aligned. */
    opo->packet = dp_packet_clone_data_with_headroom(po->packet,
                                                     po->packet_len, 2);
    /* Store struct flow. */
    opo->flow = xmalloc(sizeof *opo->flow);
    flow_extract(opo->packet, opo->flow);
    opo->flow->in_port.ofp_port = po->in_port;

    /* Check actions like for flow mods.  We pass a 'table_id' of 0 to
     * ofproto_check_consistency(), which isn't strictly correct because these
     * actions aren't in any table.  This is OK as 'table_id' is only used to
     * check instructions (e.g., goto-table), which can't appear on the action
     * list of a packet-out. */
    error = ofpacts_check_consistency(po->ofpacts, po->ofpacts_len,
                                      opo->flow,
                                      u16_to_ofp(ofproto->max_ports), 0,
                                      ofproto->n_tables,
                                      ofconn_get_protocol(ofconn));
    if (error) {
        dp_packet_delete(opo->packet);
        free(opo->flow);
        return error;
    }

    opo->ofpacts = po->ofpacts;
    opo->ofpacts_len = po->ofpacts_len;

    opo->aux = NULL;
    return 0;
}
