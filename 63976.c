format_SET_ETH_DST(const struct ofpact_mac *a, struct ds *s)
{
    ds_put_format(s, "%smod_dl_dst:%s"ETH_ADDR_FMT,
                  colors.param, colors.end, ETH_ADDR_ARGS(a->mac));
}
