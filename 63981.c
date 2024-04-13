format_SET_IP_ECN(const struct ofpact_ecn *a, struct ds *s)
{
    ds_put_format(s, "%smod_nw_ecn:%s%d",
                  colors.param, colors.end, a->ecn);
}
