ofputil_encode_switch_features(const struct ofputil_switch_features *features,
                               enum ofputil_protocol protocol, ovs_be32 xid)
{
    struct ofp_switch_features *osf;
    struct ofpbuf *b;
    enum ofp_version version;
    enum ofpraw raw;

    version = ofputil_protocol_to_ofp_version(protocol);
    switch (version) {
    case OFP10_VERSION:
        raw = OFPRAW_OFPT10_FEATURES_REPLY;
        break;
    case OFP11_VERSION:
    case OFP12_VERSION:
        raw = OFPRAW_OFPT11_FEATURES_REPLY;
        break;
    case OFP13_VERSION:
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
        raw = OFPRAW_OFPT13_FEATURES_REPLY;
        break;
    default:
        OVS_NOT_REACHED();
    }
    b = ofpraw_alloc_xid(raw, version, xid, 0);
    osf = ofpbuf_put_zeros(b, sizeof *osf);
    osf->datapath_id = htonll(features->datapath_id);
    osf->n_buffers = htonl(features->n_buffers);
    osf->n_tables = features->n_tables;

    osf->capabilities = htonl(features->capabilities &
                              ofputil_capabilities_mask(version));
    switch (version) {
    case OFP10_VERSION:
        if (features->capabilities & OFPUTIL_C_STP) {
            osf->capabilities |= htonl(OFPC10_STP);
        }
        osf->actions = ofpact_bitmap_to_openflow(features->ofpacts,
                                                 OFP10_VERSION);
        break;
    case OFP13_VERSION:
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
        osf->auxiliary_id = features->auxiliary_id;
        /* fall through */
    case OFP11_VERSION:
    case OFP12_VERSION:
        if (features->capabilities & OFPUTIL_C_GROUP_STATS) {
            osf->capabilities |= htonl(OFPC11_GROUP_STATS);
        }
        break;
    default:
        OVS_NOT_REACHED();
    }

    return b;
}
