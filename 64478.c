ofputil_decode_group_features_reply(const struct ofp_header *oh,
                                    struct ofputil_group_features *features)
{
    const struct ofp12_group_features_stats *ogf = ofpmsg_body(oh);
    int i;

    features->types = ntohl(ogf->types);
    features->capabilities = ntohl(ogf->capabilities);
    for (i = 0; i < OFPGT12_N_TYPES; i++) {
        features->max_groups[i] = ntohl(ogf->max_groups[i]);
        features->ofpacts[i] = ofpact_bitmap_from_openflow(
            ogf->actions[i], oh->version);
    }
}
