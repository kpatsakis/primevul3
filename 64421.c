decode_nx_packet_in2(const struct ofp_header *oh, bool loose,
                     const struct tun_table *tun_table,
                     const struct vl_mff_map *vl_mff_map,
                     struct ofputil_packet_in *pin,
                     size_t *total_len, uint32_t *buffer_id,
                     struct ofpbuf *continuation)
{
    *total_len = 0;
    *buffer_id = UINT32_MAX;

    struct ofpbuf properties;
    ofpbuf_use_const(&properties, oh, ntohs(oh->length));
    ofpraw_pull_assert(&properties);

    while (properties.size > 0) {
        struct ofpbuf payload;
        uint64_t type;

        enum ofperr error = ofpprop_pull(&properties, &payload, &type);
        if (error) {
            return error;
        }

        switch (type) {
        case NXPINT_PACKET:
            pin->packet = payload.msg;
            pin->packet_len = ofpbuf_msgsize(&payload);
            break;

        case NXPINT_FULL_LEN: {
            uint32_t u32;
            error = ofpprop_parse_u32(&payload, &u32);
            *total_len = u32;
            break;
        }

        case NXPINT_BUFFER_ID:
            error = ofpprop_parse_u32(&payload, buffer_id);
            break;

        case NXPINT_TABLE_ID:
            error = ofpprop_parse_u8(&payload, &pin->table_id);
            break;

        case NXPINT_COOKIE:
            error = ofpprop_parse_be64(&payload, &pin->cookie);
            break;

        case NXPINT_REASON: {
            uint8_t reason;
            error = ofpprop_parse_u8(&payload, &reason);
            pin->reason = reason;
            break;
        }

        case NXPINT_METADATA:
            error = oxm_decode_match(payload.msg, ofpbuf_msgsize(&payload),
                                     loose, tun_table, vl_mff_map,
                                     &pin->flow_metadata);
            break;

        case NXPINT_USERDATA:
            pin->userdata = payload.msg;
            pin->userdata_len = ofpbuf_msgsize(&payload);
            break;

        case NXPINT_CONTINUATION:
            if (continuation) {
                error = ofpprop_parse_nested(&payload, continuation);
            }
            break;

        default:
            error = OFPPROP_UNKNOWN(loose, "NX_PACKET_IN2", type);
            break;
        }
        if (error) {
            return error;
        }
    }

    if (!pin->packet_len) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "NXT_PACKET_IN2 lacks packet");
        return OFPERR_OFPBRC_BAD_LEN;
    } else if (!*total_len) {
        *total_len = pin->packet_len;
    } else if (*total_len < pin->packet_len) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "NXT_PACKET_IN2 claimed full_len < len");
        return OFPERR_OFPBRC_BAD_LEN;
    }

    return 0;
}
