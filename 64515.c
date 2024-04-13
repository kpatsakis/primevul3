ofputil_decode_table_desc(struct ofpbuf *msg,
                          struct ofputil_table_desc *td,
                          enum ofp_version version)
{
    memset(td, 0, sizeof *td);

    if (!msg->header) {
        ofpraw_pull_assert(msg);
    }

    if (!msg->size) {
        return EOF;
    }

    struct ofp14_table_desc *otd = ofpbuf_try_pull(msg, sizeof *otd);
    if (!otd) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "OFP14_TABLE_DESC reply has %"PRIu32" "
                     "leftover bytes at end", msg->size);
        return OFPERR_OFPBRC_BAD_LEN;
    }

    td->table_id = otd->table_id;
    size_t length = ntohs(otd->length);
    if (length < sizeof *otd || length - sizeof *otd > msg->size) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "OFP14_TABLE_DESC reply claims invalid "
                     "length %"PRIuSIZE, length);
        return OFPERR_OFPBRC_BAD_LEN;
    }
    length -= sizeof *otd;

    td->eviction = ofputil_decode_table_eviction(otd->config, version);
    td->vacancy = ofputil_decode_table_vacancy(otd->config, version);
    td->eviction_flags = UINT32_MAX;

    struct ofpbuf properties = ofpbuf_const_initializer(
        ofpbuf_pull(msg, length), length);
    while (properties.size > 0) {
        struct ofpbuf payload;
        enum ofperr error;
        uint64_t type;

        error = ofpprop_pull(&properties, &payload, &type);
        if (error) {
            return error;
        }

        switch (type) {
        case OFPTMPT14_EVICTION:
            error = ofpprop_parse_u32(&payload, &td->eviction_flags);
            break;

        case OFPTMPT14_VACANCY:
            error = parse_table_desc_vacancy_property(&payload, td);
            break;

        default:
            error = OFPPROP_UNKNOWN(true, "table_desc", type);
            break;
        }

        if (error) {
            return error;
        }
    }

    return 0;
}
