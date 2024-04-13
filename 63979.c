format_SET_IPV4_SRC(const struct ofpact_ipv4 *a, struct ds *s)
{
    ds_put_format(s, "%smod_nw_src:%s"IP_FMT,
                  colors.param, colors.end, IP_ARGS(a->ipv4));
}
