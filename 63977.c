format_SET_ETH_SRC(const struct ofpact_mac *a, struct ds *s)
{
    ds_put_format(s, "%smod_dl_src:%s"ETH_ADDR_FMT,
                  colors.param, colors.end, ETH_ADDR_ARGS(a->mac));
}
