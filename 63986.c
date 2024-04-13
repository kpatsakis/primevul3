format_SET_MPLS_TC(const struct ofpact_mpls_tc *a, struct ds *s)
{
    ds_put_format(s, "%sset_mpls_ttl(%s%"PRIu8"%s)%s",
                  colors.paren, colors.end, a->tc,
                  colors.paren, colors.end);
}
