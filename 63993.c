format_UNROLL_XLATE(const struct ofpact_unroll_xlate *a, struct ds *s)
{
    ds_put_format(s, "%sunroll_xlate(%s%stable=%s%"PRIu8
                  ", %scookie=%s%"PRIu64"%s)%s",
                  colors.paren,   colors.end,
                  colors.special, colors.end, a->rule_table_id,
                  colors.param,   colors.end, ntohll(a->rule_cookie),
                  colors.paren,   colors.end);
}
