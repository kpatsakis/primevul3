format_OUTPUT_REG(const struct ofpact_output_reg *a, struct ds *s)
{
    ds_put_format(s, "%soutput:%s", colors.special, colors.end);
    mf_format_subfield(&a->src, s);
}
