copy_buckets_for_insert_bucket(const struct ofgroup *ofgroup,
                               struct ofgroup *new_ofgroup,
                               uint32_t command_bucket_id)
{
    struct ofputil_bucket *last = NULL;

    if (command_bucket_id <= OFPG15_BUCKET_MAX) {
        /* Check here to ensure that a bucket corresponding to
         * command_bucket_id exists in the old bucket list.
         *
         * The subsequent search of below of new_ofgroup covers
         * both buckets in the old bucket list and buckets added
         * by the insert buckets group mod message this function processes. */
        if (!ofputil_bucket_find(&ofgroup->buckets, command_bucket_id)) {
            return OFPERR_OFPGMFC_UNKNOWN_BUCKET;
        }

        if (!ovs_list_is_empty(&new_ofgroup->buckets)) {
            last = ofputil_bucket_list_back(&new_ofgroup->buckets);
        }
    }

    ofputil_bucket_clone_list(CONST_CAST(struct ovs_list *,
                                         &new_ofgroup->buckets),
                              &ofgroup->buckets, NULL);

    if (ofputil_bucket_check_duplicate_id(&new_ofgroup->buckets)) {
            VLOG_INFO_RL(&rl, "Duplicate bucket id");
            return OFPERR_OFPGMFC_BUCKET_EXISTS;
    }

    /* Rearrange list according to command_bucket_id */
    if (command_bucket_id == OFPG15_BUCKET_LAST) {
        if (!ovs_list_is_empty(&ofgroup->buckets)) {
            struct ofputil_bucket *new_first;
            const struct ofputil_bucket *first;

            first = ofputil_bucket_list_front(&ofgroup->buckets);
            new_first = ofputil_bucket_find(&new_ofgroup->buckets,
                                            first->bucket_id);

            ovs_list_splice(new_ofgroup->buckets.next, &new_first->list_node,
                            CONST_CAST(struct ovs_list *,
                                       &new_ofgroup->buckets));
        }
    } else if (command_bucket_id <= OFPG15_BUCKET_MAX && last) {
        struct ofputil_bucket *after;

        /* Presence of bucket is checked above so after should never be NULL */
        after = ofputil_bucket_find(&new_ofgroup->buckets, command_bucket_id);

        ovs_list_splice(after->list_node.next, new_ofgroup->buckets.next,
                    last->list_node.next);
    }

    return 0;
}
