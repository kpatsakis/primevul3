ofputil_encode_ofp11_group_mod(enum ofp_version ofp_version,
                               const struct ofputil_group_mod *gm)
{
    struct ofpbuf *b;
    struct ofp11_group_mod *ogm;
    size_t start_ogm;
    struct ofputil_bucket *bucket;

    b = ofpraw_alloc(OFPRAW_OFPT11_GROUP_MOD, ofp_version, 0);
    start_ogm = b->size;
    ofpbuf_put_zeros(b, sizeof *ogm);

    LIST_FOR_EACH (bucket, list_node, &gm->buckets) {
        ofputil_put_ofp11_bucket(bucket, b, ofp_version);
    }
    ogm = ofpbuf_at_assert(b, start_ogm, sizeof *ogm);
    ogm->command = htons(gm->command);
    ogm->type = gm->type;
    ogm->group_id = htonl(gm->group_id);

    return b;
}
