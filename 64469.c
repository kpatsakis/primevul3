ofputil_decode_flow_monitor_cancel(const struct ofp_header *oh)
{
    const struct nx_flow_monitor_cancel *cancel = ofpmsg_body(oh);

    return ntohl(cancel->id);
}
