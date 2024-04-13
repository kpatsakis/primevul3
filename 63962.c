format_GROUP(const struct ofpact_group *a, struct ds *s)
{
    ds_put_format(s, "%sgroup:%s%"PRIu32,
                  colors.special, colors.end, a->group_id);
}
