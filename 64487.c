ofputil_decode_meter_request(const struct ofp_header *oh, uint32_t *meter_id)
{
    const struct ofp13_meter_multipart_request *omr = ofpmsg_body(oh);
    *meter_id = ntohl(omr->meter_id);
}
