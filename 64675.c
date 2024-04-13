ofputil_uninit_group_mod(struct ofputil_group_mod *gm)
{
    ofputil_bucket_list_destroy(&gm->buckets);
    ofputil_group_properties_destroy(&gm->props);
}
