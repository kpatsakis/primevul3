    OVS_REQUIRES(ofproto_mutex)
{
    delete_flows_revert__(ofproto, &ofm->old_rules);
}
