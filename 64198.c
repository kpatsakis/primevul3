    OVS_REQUIRES(ofproto_mutex)
{
    /* Makes flow deletion code leave the rule pointers in 'group->rules'
     * intact, so that we can later refer to the rules deleted due to the group
     * deletion.  Rule pointers will be removed from all other groups, if any,
     * so we will never try to delete the same rule twice. */
    group->being_deleted = true;

    /* Mark all the referring groups for deletion. */
    delete_flows_start__(ofproto, version, &group->rules);
    group_collection_add(groups, group);
    versions_set_remove_version(&group->versions, version);
    ofproto->n_groups[group->type]--;
}
