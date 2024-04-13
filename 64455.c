ofputil_bucket_check_duplicate_id(const struct ovs_list *buckets)
{
    struct ofputil_bucket *i, *j;

    LIST_FOR_EACH (i, list_node, buckets) {
        LIST_FOR_EACH_REVERSE (j, list_node, buckets) {
            if (i == j) {
                break;
            }
            if (i->bucket_id == j->bucket_id) {
                return true;
            }
        }
    }

    return false;
}
