get_provider_meter_id(const struct ofproto *ofproto, uint32_t of_meter_id)
{
    if (of_meter_id && of_meter_id <= ofproto->meter_features.max_meters) {
        const struct meter *meter = ofproto->meters[of_meter_id];
        if (meter) {
            return meter->provider_meter_id.uint32;
        }
    }
    return UINT32_MAX;
}
