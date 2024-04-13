ofputil_encode_meter_request(enum ofp_version ofp_version,
                             enum ofputil_meter_request_type type,
                             uint32_t meter_id)
{
    struct ofpbuf *msg;

    enum ofpraw raw;

    switch (type) {
    case OFPUTIL_METER_CONFIG:
        raw = OFPRAW_OFPST13_METER_CONFIG_REQUEST;
        break;
    case OFPUTIL_METER_STATS:
        raw = OFPRAW_OFPST13_METER_REQUEST;
        break;
    default:
    case OFPUTIL_METER_FEATURES:
        raw = OFPRAW_OFPST13_METER_FEATURES_REQUEST;
        break;
    }

    msg = ofpraw_alloc(raw, ofp_version, 0);

    if (type != OFPUTIL_METER_FEATURES) {
        struct ofp13_meter_multipart_request *omr;
        omr = ofpbuf_put_zeros(msg, sizeof *omr);
        omr->meter_id = htonl(meter_id);
    }
    return msg;
}
