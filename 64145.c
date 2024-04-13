    OVS_EXCLUDED(ofproto_mutex)
{
    enum ofperr error;
    uint32_t id;

    id = ofputil_decode_flow_monitor_cancel(oh);

    ovs_mutex_lock(&ofproto_mutex);
    error = flow_monitor_delete(ofconn, id);
    ovs_mutex_unlock(&ofproto_mutex);

    return error;
}
