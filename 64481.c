ofputil_decode_group_stats_request(const struct ofp_header *request,
                                   uint32_t *group_id)
{
    const struct ofp11_group_stats_request *gsr11 = ofpmsg_body(request);
    *group_id = ntohl(gsr11->group_id);
    return 0;
}
