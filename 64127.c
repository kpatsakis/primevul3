    OVS_EXCLUDED(ofproto_mutex)
{
    ovsrcu_postpone(ofproto_destroy__, ofproto);
}
