ofputil_pull_switch_features(struct ofpbuf *b,
                             struct ofputil_switch_features *features)
{
    const struct ofp_header *oh = b->data;
    enum ofpraw raw = ofpraw_pull_assert(b);
    const struct ofp_switch_features *osf = ofpbuf_pull(b, sizeof *osf);
    features->datapath_id = ntohll(osf->datapath_id);
    features->n_buffers = ntohl(osf->n_buffers);
    features->n_tables = osf->n_tables;
    features->auxiliary_id = 0;

    features->capabilities = ntohl(osf->capabilities) &
        ofputil_capabilities_mask(oh->version);

    if (raw == OFPRAW_OFPT10_FEATURES_REPLY) {
        if (osf->capabilities & htonl(OFPC10_STP)) {
            features->capabilities |= OFPUTIL_C_STP;
        }
        features->ofpacts = ofpact_bitmap_from_openflow(osf->actions,
                                                        OFP10_VERSION);
    } else if (raw == OFPRAW_OFPT11_FEATURES_REPLY
               || raw == OFPRAW_OFPT13_FEATURES_REPLY) {
        if (osf->capabilities & htonl(OFPC11_GROUP_STATS)) {
            features->capabilities |= OFPUTIL_C_GROUP_STATS;
        }
        features->ofpacts = 0;
        if (raw == OFPRAW_OFPT13_FEATURES_REPLY) {
            features->auxiliary_id = osf->auxiliary_id;
        }
    } else {
        return OFPERR_OFPBRC_BAD_VERSION;
    }

    return 0;
}
