meter_create(const struct ofputil_meter_config *config,
             ofproto_meter_id provider_meter_id)
{
    struct meter *meter;

    meter = xzalloc(sizeof *meter);
    meter->provider_meter_id = provider_meter_id;
    meter->created = time_msec();
    ovs_list_init(&meter->rules);

    meter_update(meter, config);

    return meter;
}
