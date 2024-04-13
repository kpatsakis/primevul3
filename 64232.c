handle_add_meter(struct ofproto *ofproto, struct ofputil_meter_mod *mm)
{
    ofproto_meter_id provider_meter_id = { UINT32_MAX };
    struct meter **meterp = &ofproto->meters[mm->meter.meter_id];
    enum ofperr error;

    if (*meterp) {
        return OFPERR_OFPMMFC_METER_EXISTS;
    }

    error = ofproto->ofproto_class->meter_set(ofproto, &provider_meter_id,
                                              &mm->meter);
    if (!error) {
        ovs_assert(provider_meter_id.uint32 != UINT32_MAX);
        *meterp = meter_create(&mm->meter, provider_meter_id);
    }
    return error;
}
