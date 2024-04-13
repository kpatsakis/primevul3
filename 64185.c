    OVS_REQUIRES(ofproto_mutex)
{
    delete_flows_finish__(ofproto, &ofm->old_rules, OFPRR_DELETE, req);
}
