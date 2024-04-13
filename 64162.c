    OVS_REQUIRES(ofproto_mutex)
{
    enum ofperr error;

    error = ofproto_check_ofpacts(ofproto, opo->ofpacts, opo->ofpacts_len);
    if (error) {
        return error;
    }

    return ofproto->ofproto_class->packet_xlate(ofproto, opo);
}
