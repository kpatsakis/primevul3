format_EXIT(const struct ofpact_null *a OVS_UNUSED, struct ds *s)
{
    ds_put_format(s, "%sexit%s", colors.special, colors.end);
}
