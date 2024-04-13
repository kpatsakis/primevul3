ofproto_group_ref(struct ofgroup *group)
{
    if (group) {
        ovs_refcount_ref(&group->ref_count);
    }
}
