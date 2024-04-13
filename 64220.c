copy_buckets_for_remove_bucket(const struct ofgroup *ofgroup,
                               struct ofgroup *new_ofgroup,
                               uint32_t command_bucket_id)
{
    const struct ofputil_bucket *skip = NULL;

    if (command_bucket_id == OFPG15_BUCKET_ALL) {
        return 0;
    }

    if (command_bucket_id == OFPG15_BUCKET_FIRST) {
        if (!ovs_list_is_empty(&ofgroup->buckets)) {
            skip = ofputil_bucket_list_front(&ofgroup->buckets);
        }
    } else if (command_bucket_id == OFPG15_BUCKET_LAST) {
        if (!ovs_list_is_empty(&ofgroup->buckets)) {
            skip = ofputil_bucket_list_back(&ofgroup->buckets);
        }
    } else {
        skip = ofputil_bucket_find(&ofgroup->buckets, command_bucket_id);
        if (!skip) {
            return OFPERR_OFPGMFC_UNKNOWN_BUCKET;
        }
    }

    ofputil_bucket_clone_list(CONST_CAST(struct ovs_list *,
                                         &new_ofgroup->buckets),
                              &ofgroup->buckets, skip);

    return 0;
}
