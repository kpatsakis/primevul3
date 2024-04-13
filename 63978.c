format_SET_IPV4_DST(const struct ofpact_ipv4 *a, struct ds *s)
{
    ds_put_format(s, "%smod_nw_dst:%s"IP_FMT,
                  colors.param, colors.end, IP_ARGS(a->ipv4));
}
