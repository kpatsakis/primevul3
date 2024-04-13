group_destroy_cb(struct ofgroup *group)
{
    group->ofproto->ofproto_class->group_destruct(group);
    ofputil_group_properties_destroy(CONST_CAST(struct ofputil_group_props *,
                                                &group->props));
    ofputil_bucket_list_destroy(CONST_CAST(struct ovs_list *,
                                           &group->buckets));
    group->ofproto->ofproto_class->group_dealloc(group);
}
