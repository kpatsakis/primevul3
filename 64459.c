ofputil_bucket_list_destroy(struct ovs_list *buckets)
{
    struct ofputil_bucket *bucket;

    LIST_FOR_EACH_POP (bucket, list_node, buckets) {
        free(bucket->ofpacts);
        free(bucket);
    }
}
