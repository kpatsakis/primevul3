format_NOTE(const struct ofpact_note *a, struct ds *s)
{
    ds_put_format(s, "%snote:%s", colors.param, colors.end);
    format_hex_arg(s, a->data, a->length);
}
