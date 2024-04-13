    OVS_REQUIRES(ofproto_mutex)
{
    ofproto->ofproto_class->packet_xlate_revert(ofproto, opo);
}
