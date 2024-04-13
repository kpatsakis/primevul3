format_SET_IP_DSCP(const struct ofpact_dscp *a, struct ds *s)
{
    ds_put_format(s, "%smod_nw_tos:%s%d", colors.param, colors.end, a->dscp);
}
