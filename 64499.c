ofputil_decode_packet_in(const struct ofp_header *oh, bool loose,
                         const struct tun_table *tun_table,
                         const struct vl_mff_map *vl_mff_map,
                         struct ofputil_packet_in *pin,
                         size_t *total_lenp, uint32_t *buffer_idp,
                         struct ofpbuf *continuation)
{
    uint32_t buffer_id;
    size_t total_len;

    memset(pin, 0, sizeof *pin);
    pin->cookie = OVS_BE64_MAX;
    if (continuation) {
        ofpbuf_use_const(continuation, NULL, 0);
    }

    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);
    if (raw == OFPRAW_OFPT13_PACKET_IN || raw == OFPRAW_OFPT12_PACKET_IN) {
        const struct ofp12_packet_in *opi = ofpbuf_pull(&b, sizeof *opi);
        const ovs_be64 *cookie = (raw == OFPRAW_OFPT13_PACKET_IN
                                  ? ofpbuf_pull(&b, sizeof *cookie)
                                  : NULL);
        enum ofperr error = oxm_pull_match_loose(&b, tun_table,
                                                 &pin->flow_metadata);
        if (error) {
            return error;
        }

        if (!ofpbuf_try_pull(&b, 2)) {
            return OFPERR_OFPBRC_BAD_LEN;
        }

        pin->reason = opi->reason;
        pin->table_id = opi->table_id;
        buffer_id = ntohl(opi->buffer_id);
        total_len = ntohs(opi->total_len);
        if (cookie) {
            pin->cookie = *cookie;
        }

        pin->packet = b.data;
        pin->packet_len = b.size;
    } else if (raw == OFPRAW_OFPT10_PACKET_IN) {
        const struct ofp10_packet_in *opi;

        opi = ofpbuf_pull(&b, offsetof(struct ofp10_packet_in, data));

        pin->packet = CONST_CAST(uint8_t *, opi->data);
        pin->packet_len = b.size;

        match_init_catchall(&pin->flow_metadata);
        match_set_in_port(&pin->flow_metadata,
                          u16_to_ofp(ntohs(opi->in_port)));
        pin->reason = opi->reason;
        buffer_id = ntohl(opi->buffer_id);
        total_len = ntohs(opi->total_len);
    } else if (raw == OFPRAW_OFPT11_PACKET_IN) {
        const struct ofp11_packet_in *opi;
        ofp_port_t in_port;
        enum ofperr error;

        opi = ofpbuf_pull(&b, sizeof *opi);

        pin->packet = b.data;
        pin->packet_len = b.size;

        buffer_id = ntohl(opi->buffer_id);
        error = ofputil_port_from_ofp11(opi->in_port, &in_port);
        if (error) {
            return error;
        }
        match_init_catchall(&pin->flow_metadata);
        match_set_in_port(&pin->flow_metadata, in_port);
        total_len = ntohs(opi->total_len);
        pin->reason = opi->reason;
        pin->table_id = opi->table_id;
    } else if (raw == OFPRAW_NXT_PACKET_IN) {
        const struct nx_packet_in *npi;
        int error;

        npi = ofpbuf_pull(&b, sizeof *npi);
        error = nx_pull_match_loose(&b, ntohs(npi->match_len),
                                    &pin->flow_metadata, NULL, NULL, NULL);
        if (error) {
            return error;
        }

        if (!ofpbuf_try_pull(&b, 2)) {
            return OFPERR_OFPBRC_BAD_LEN;
        }

        pin->reason = npi->reason;
        pin->table_id = npi->table_id;
        pin->cookie = npi->cookie;

        buffer_id = ntohl(npi->buffer_id);
        total_len = ntohs(npi->total_len);

        pin->packet = b.data;
        pin->packet_len = b.size;
    } else if (raw == OFPRAW_NXT_PACKET_IN2 || raw == OFPRAW_NXT_RESUME) {
        enum ofperr error = decode_nx_packet_in2(oh, loose, tun_table,
                                                 vl_mff_map, pin, &total_len,
                                                 &buffer_id, continuation);
        if (error) {
            return error;
        }
    } else {
        OVS_NOT_REACHED();
    }

    if (total_lenp) {
        *total_lenp = total_len;
    }
    if (buffer_idp) {
        *buffer_idp = buffer_id;
    }

    return 0;
}
