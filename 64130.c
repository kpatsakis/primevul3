    OVS_EXCLUDED(ofproto_mutex)
{
    return handle_flow_mod__(ofproto, fm, NULL);
}
