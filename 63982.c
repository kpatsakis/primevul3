format_SET_IP_TTL(const struct ofpact_ip_ttl *a, struct ds *s)
{
    ds_put_format(s, "%smod_nw_ttl:%s%d", colors.param, colors.end, a->ttl);
}
