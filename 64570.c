ofputil_encode_table_desc_request(enum ofp_version ofp_version)
{
    struct ofpbuf *request = NULL;

    if (ofp_version >= OFP14_VERSION) {
        request = ofpraw_alloc(OFPRAW_OFPST14_TABLE_DESC_REQUEST,
                               ofp_version, 0);
    } else {
        ovs_fatal(0, "dump-table-desc needs OpenFlow 1.4 or later "
                  "(\'-O OpenFlow14\')");
    }

    return request;
}
