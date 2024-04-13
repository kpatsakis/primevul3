format_METER(const struct ofpact_meter *a, struct ds *s)
{
    ds_put_format(s, "%smeter:%s%"PRIu32,
                  colors.param, colors.end, a->meter_id);
}
