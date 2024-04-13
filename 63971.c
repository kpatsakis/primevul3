format_POP_QUEUE(const struct ofpact_null *a OVS_UNUSED, struct ds *s)
{
    ds_put_format(s, "%spop_queue%s", colors.value, colors.end);
}
