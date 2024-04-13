format_SET_MPLS_TTL(const struct ofpact_mpls_ttl *a, struct ds *s)
{
    ds_put_format(s, "%sset_mpls_ttl(%s%"PRIu8"%s)%s",
                  colors.paren, colors.end, a->ttl,
                  colors.paren, colors.end);
}
