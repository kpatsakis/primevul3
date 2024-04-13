format_DEBUG_RECIRC(const struct ofpact_null *a OVS_UNUSED, struct ds *s)
{
    ds_put_format(s, "%sdebug_recirc%s", colors.value, colors.end);
}
