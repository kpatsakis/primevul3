ofproto_group_try_ref(struct ofgroup *group)
{
    if (group) {
        return ovs_refcount_try_ref_rcu(&group->ref_count);
    }
    return false;
}
