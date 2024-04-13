format_SET_MPLS_LABEL(const struct ofpact_mpls_label *a, struct ds *s)
{
    ds_put_format(s, "%sset_mpls_label(%s%"PRIu32"%s)%s",
                  colors.paren, colors.end, ntohl(a->label),
                  colors.paren, colors.end);
}
