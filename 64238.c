handle_group_stats_request(struct ofconn *ofconn,
                           const struct ofp_header *request)
{
    uint32_t group_id;
    enum ofperr error;

    error = ofputil_decode_group_stats_request(request, &group_id);
    if (error) {
        return error;
    }

    handle_group_request(ofconn, request, group_id, append_group_stats);
    return 0;
}
