    OVS_REQUIRES(ofproto_mutex)
{
    ofproto->ofproto_class->packet_execute(ofproto, opo);
}
