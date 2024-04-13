format_CT_CLEAR(const struct ofpact_null *a OVS_UNUSED, struct ds *s)
{
    ds_put_format(s, "%sct_clear%s", colors.value, colors.end);
}
