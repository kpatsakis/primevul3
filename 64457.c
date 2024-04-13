ofputil_bucket_find(const struct ovs_list *buckets, uint32_t bucket_id)
{
    struct ofputil_bucket *bucket;

    if (bucket_id > OFPG15_BUCKET_MAX) {
        return NULL;
    }

    LIST_FOR_EACH (bucket, list_node, buckets) {
        if (bucket->bucket_id == bucket_id) {
            return bucket;
        }
    }

    return NULL;
}
