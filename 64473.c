ofputil_decode_flow_stats_request(struct ofputil_flow_stats_request *fsr,
                                  const struct ofp_header *oh,
                                  const struct tun_table *tun_table,
                                  const struct vl_mff_map *vl_mff_map)
{
    struct ofpbuf b = ofpbuf_const_initializer(oh, ntohs(oh->length));
    enum ofpraw raw = ofpraw_pull_assert(&b);
    switch ((int) raw) {
    case OFPRAW_OFPST10_FLOW_REQUEST:
        return ofputil_decode_ofpst10_flow_request(fsr, b.data, false);

    case OFPRAW_OFPST10_AGGREGATE_REQUEST:
        return ofputil_decode_ofpst10_flow_request(fsr, b.data, true);

    case OFPRAW_OFPST11_FLOW_REQUEST:
        return ofputil_decode_ofpst11_flow_request(fsr, &b, false, tun_table,
                                                   vl_mff_map);

    case OFPRAW_OFPST11_AGGREGATE_REQUEST:
        return ofputil_decode_ofpst11_flow_request(fsr, &b, true, tun_table,
                                                   vl_mff_map);

    case OFPRAW_NXST_FLOW_REQUEST:
        return ofputil_decode_nxst_flow_request(fsr, &b, false, tun_table,
                                                vl_mff_map);

    case OFPRAW_NXST_AGGREGATE_REQUEST:
        return ofputil_decode_nxst_flow_request(fsr, &b, true, tun_table,
                                                vl_mff_map);

    default:
        /* Hey, the caller lied. */
        OVS_NOT_REACHED();
    }
}
