ofputil_encode_packet_out(const struct ofputil_packet_out *po,
                          enum ofputil_protocol protocol)
{
    enum ofp_version ofp_version = ofputil_protocol_to_ofp_version(protocol);
    struct ofpbuf *msg;
    size_t size;

    size = po->ofpacts_len;
    if (po->buffer_id == UINT32_MAX) {
        size += po->packet_len;
    }

    switch (ofp_version) {
    case OFP10_VERSION: {
        struct ofp10_packet_out *opo;
        size_t actions_ofs;

        msg = ofpraw_alloc(OFPRAW_OFPT10_PACKET_OUT, OFP10_VERSION, size);
        ofpbuf_put_zeros(msg, sizeof *opo);
        actions_ofs = msg->size;
        ofpacts_put_openflow_actions(po->ofpacts, po->ofpacts_len, msg,
                                     ofp_version);

        opo = msg->msg;
        opo->buffer_id = htonl(po->buffer_id);
        opo->in_port = htons(ofp_to_u16(po->in_port));
        opo->actions_len = htons(msg->size - actions_ofs);
        break;
    }

    case OFP11_VERSION:
    case OFP12_VERSION:
    case OFP13_VERSION:
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION: {
        struct ofp11_packet_out *opo;
        size_t len;

        msg = ofpraw_alloc(OFPRAW_OFPT11_PACKET_OUT, ofp_version, size);
        ofpbuf_put_zeros(msg, sizeof *opo);
        len = ofpacts_put_openflow_actions(po->ofpacts, po->ofpacts_len, msg,
                                           ofp_version);
        opo = msg->msg;
        opo->buffer_id = htonl(po->buffer_id);
        opo->in_port = ofputil_port_to_ofp11(po->in_port);
        opo->actions_len = htons(len);
        break;
    }

    default:
        OVS_NOT_REACHED();
    }

    if (po->buffer_id == UINT32_MAX) {
        ofpbuf_put(msg, po->packet, po->packet_len);
    }

    ofpmsg_update_length(msg);

    return msg;
}
