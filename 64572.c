ofputil_encode_table_stats_reply(const struct ofp_header *request)
{
    return ofpraw_alloc_stats_reply(request, 0);
}
