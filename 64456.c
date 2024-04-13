ofputil_bucket_clone_list(struct ovs_list *dest, const struct ovs_list *src,
                          const struct ofputil_bucket *skip)
{
    struct ofputil_bucket *bucket;

    LIST_FOR_EACH (bucket, list_node, src) {
        struct ofputil_bucket *new_bucket;

        if (bucket == skip) {
            continue;
        }

        new_bucket = ofputil_bucket_clone_data(bucket);
        ovs_list_push_back(dest, &new_bucket->list_node);
    }
}
