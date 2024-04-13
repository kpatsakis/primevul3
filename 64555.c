ofputil_encode_ofp15_group_mod(enum ofp_version ofp_version,
                               const struct ofputil_group_mod *gm)
{
    struct ofpbuf *b;
    struct ofp15_group_mod *ogm;
    size_t start_ogm;
    struct ofputil_bucket *bucket;
    struct id_pool *bucket_ids = NULL;

    b = ofpraw_alloc(OFPRAW_OFPT15_GROUP_MOD, ofp_version, 0);
    start_ogm = b->size;
    ofpbuf_put_zeros(b, sizeof *ogm);

    LIST_FOR_EACH (bucket, list_node, &gm->buckets) {
        uint32_t bucket_id;

        /* Generate a bucket id if none was supplied */
        if (bucket->bucket_id > OFPG15_BUCKET_MAX) {
            if (!bucket_ids) {
                const struct ofputil_bucket *bkt;

                bucket_ids = id_pool_create(0, OFPG15_BUCKET_MAX + 1);

                /* Mark all bucket_ids that are present in gm
                 * as used in the pool. */
                LIST_FOR_EACH_REVERSE (bkt, list_node, &gm->buckets) {
                    if (bkt == bucket) {
                        break;
                    }
                    if (bkt->bucket_id <= OFPG15_BUCKET_MAX) {
                        id_pool_add(bucket_ids, bkt->bucket_id);
                    }
                }
            }

            if (!id_pool_alloc_id(bucket_ids, &bucket_id)) {
                OVS_NOT_REACHED();
            }
        } else {
            bucket_id = bucket->bucket_id;
        }

        ofputil_put_ofp15_bucket(bucket, bucket_id, gm->type, b, ofp_version);
    }
    ogm = ofpbuf_at_assert(b, start_ogm, sizeof *ogm);
    ogm->command = htons(gm->command);
    ogm->type = gm->type;
    ogm->group_id = htonl(gm->group_id);
    ogm->command_bucket_id = htonl(gm->command_bucket_id);
    ogm->bucket_array_len = htons(b->size - start_ogm - sizeof *ogm);

    /* Add group properties */
    if (gm->props.selection_method[0]) {
        ofputil_put_group_prop_ntr_selection_method(ofp_version, &gm->props, b);
    }

    id_pool_destroy(bucket_ids);
    return b;
}
