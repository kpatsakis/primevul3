make_echo_request(enum ofp_version ofp_version)
{
    return ofpraw_alloc_xid(OFPRAW_OFPT_ECHO_REQUEST, ofp_version,
                            htonl(0), 0);
}
