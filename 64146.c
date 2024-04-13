    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofproto *ofproto = ofconn_get_ofproto(ofconn);
    uint32_t meter_id = mm->meter.meter_id;
    enum ofperr error = 0;
    uint32_t first, last;

    if (meter_id == OFPM13_ALL) {
        first = 1;
        last = ofproto->meter_features.max_meters;
    } else {
        if (!meter_id || meter_id > ofproto->meter_features.max_meters) {
            return 0;
        }
        first = last = meter_id;
    }

    /* Delete the meters. */
    ovs_mutex_lock(&ofproto_mutex);
    meter_delete(ofproto, first, last);
    ovs_mutex_unlock(&ofproto_mutex);

    return error;
}
