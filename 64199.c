    OVS_REQUIRES(ofproto_mutex)
{
    /* Finish deletion of all flow entries containing this group in a group
     * action. */
    delete_flows_finish__(ofproto, &group->rules, OFPRR_GROUP_DELETE, NULL);

    /* Group removal is postponed by the caller. */
}
