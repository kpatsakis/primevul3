ofputil_encode_bundle_msgs(const struct ofputil_bundle_msg *bms,
                           size_t n_bms, struct ovs_list *requests,
                           enum ofputil_protocol protocol)
{
    enum ofp_version version = ofputil_protocol_to_ofp_version(protocol);

    for (size_t i = 0; i < n_bms; i++) {
        struct ofpbuf *request = NULL;

        switch ((int)bms[i].type) {
        case OFPTYPE_FLOW_MOD:
            request = ofputil_encode_flow_mod(&bms[i].fm, protocol);
            break;
        case OFPTYPE_GROUP_MOD:
            request = ofputil_encode_group_mod(version, &bms[i].gm);
            break;
        case OFPTYPE_PACKET_OUT:
            request = ofputil_encode_packet_out(&bms[i].po, protocol);
            break;
        default:
            break;
        }
        if (request) {
            ovs_list_push_back(requests, &request->list_node);
        }
    }
}
