ofputil_bucket_list_back(const struct ovs_list *buckets)
{
    static struct ofputil_bucket *bucket;

    ASSIGN_CONTAINER(bucket, ovs_list_back(buckets), list_node);

    return bucket;
}
