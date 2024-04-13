ofputil_encode_group_features_reply(
    const struct ofputil_group_features *features,
    const struct ofp_header *request)
{
    struct ofp12_group_features_stats *ogf;
    struct ofpbuf *reply;
    int i;

    reply = ofpraw_alloc_xid(OFPRAW_OFPST12_GROUP_FEATURES_REPLY,
                             request->version, request->xid, 0);
    ogf = ofpbuf_put_zeros(reply, sizeof *ogf);
    ogf->types = htonl(features->types);
    ogf->capabilities = htonl(features->capabilities);
    for (i = 0; i < OFPGT12_N_TYPES; i++) {
        ogf->max_groups[i] = htonl(features->max_groups[i]);
        ogf->actions[i] = ofpact_bitmap_to_openflow(features->ofpacts[i],
                                                    request->version);
    }

    return reply;
}
