ofputil_decode_packet_out(struct ofputil_packet_out *po,
                          const struct ofp_header *oh,
                          struct ofpbuf *ofpacts)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);

    ofpbuf_clear(ofpacts);
    if (raw == OFPRAW_OFPT11_PACKET_OUT) {
        enum ofperr error;
        const struct ofp11_packet_out *opo = ofpbuf_pull(&b, sizeof *opo);

        po->buffer_id = ntohl(opo->buffer_id);
        error = ofputil_port_from_ofp11(opo->in_port, &po->in_port);
        if (error) {
            return error;
        }

        error = ofpacts_pull_openflow_actions(&b, ntohs(opo->actions_len),
                                              oh->version, NULL, NULL,
                                              ofpacts);
        if (error) {
            return error;
        }
    } else if (raw == OFPRAW_OFPT10_PACKET_OUT) {
        enum ofperr error;
        const struct ofp10_packet_out *opo = ofpbuf_pull(&b, sizeof *opo);

        po->buffer_id = ntohl(opo->buffer_id);
        po->in_port = u16_to_ofp(ntohs(opo->in_port));

        error = ofpacts_pull_openflow_actions(&b, ntohs(opo->actions_len),
                                              oh->version, NULL, NULL,
                                              ofpacts);
        if (error) {
            return error;
        }
    } else {
        OVS_NOT_REACHED();
    }

    if (ofp_to_u16(po->in_port) >= ofp_to_u16(OFPP_MAX)
        && po->in_port != OFPP_LOCAL
        && po->in_port != OFPP_NONE && po->in_port != OFPP_CONTROLLER) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "packet-out has bad input port %#"PRIx32,
                     po->in_port);
        return OFPERR_OFPBRC_BAD_PORT;
    }

    po->ofpacts = ofpacts->data;
    po->ofpacts_len = ofpacts->size;

    if (po->buffer_id == UINT32_MAX) {
        po->packet = b.data;
        po->packet_len = b.size;
    } else {
        po->packet = NULL;
        po->packet_len = 0;
    }

    return 0;
}
