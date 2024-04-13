append_group_desc(struct ofgroup *group, struct ovs_list *replies)
{
    struct ofputil_group_desc gds;

    gds.group_id = group->group_id;
    gds.type = group->type;
    gds.props = group->props;

    ofputil_append_group_desc_reply(&gds, &group->buckets, replies);
}
