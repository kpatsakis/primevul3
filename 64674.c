ofputil_uninit_group_desc(struct ofputil_group_desc *gd)
{
    ofputil_bucket_list_destroy(&gd->buckets);
    ofputil_group_properties_destroy(&gd->props);
}
