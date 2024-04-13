ofputil_decode_group_desc_request(const struct ofp_header *oh)
{
    struct ofpbuf request = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&request);
    if (raw == OFPRAW_OFPST11_GROUP_DESC_REQUEST) {
        return OFPG_ALL;
    } else if (raw == OFPRAW_OFPST15_GROUP_DESC_REQUEST) {
        ovs_be32 *group_id = ofpbuf_pull(&request, sizeof *group_id);
        return ntohl(*group_id);
    } else {
        OVS_NOT_REACHED();
    }
}
