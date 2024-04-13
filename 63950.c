format_CLEAR_ACTIONS(const struct ofpact_null *a OVS_UNUSED, struct ds *s)
{
    ds_put_format(s, "%sclear_actions%s", colors.value, colors.end);
}
