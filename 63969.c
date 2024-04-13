format_OUTPUT_TRUNC(const struct ofpact_output_trunc *a, struct ds *s)
{
     ds_put_format(s, "%soutput%s(port=%"PRIu32",max_len=%"PRIu32")",
                   colors.special, colors.end, a->port, a->max_len);
}
