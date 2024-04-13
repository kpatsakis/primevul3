handle_modify_meter(struct ofproto *ofproto, struct ofputil_meter_mod *mm)
{
    struct meter *meter = ofproto->meters[mm->meter.meter_id];
    enum ofperr error;
    uint32_t provider_meter_id;

    if (!meter) {
        return OFPERR_OFPMMFC_UNKNOWN_METER;
    }

    provider_meter_id = meter->provider_meter_id.uint32;
    error = ofproto->ofproto_class->meter_set(ofproto,
                                              &meter->provider_meter_id,
                                              &mm->meter);
    ovs_assert(meter->provider_meter_id.uint32 == provider_meter_id);
    if (!error) {
        meter_update(meter, &mm->meter);
    }
    return error;
}
