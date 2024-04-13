format_FIN_TIMEOUT(const struct ofpact_fin_timeout *a, struct ds *s)
{
    ds_put_format(s, "%sfin_timeout(%s", colors.paren, colors.end);
    if (a->fin_idle_timeout) {
        ds_put_format(s, "%sidle_timeout=%s%"PRIu16",",
                      colors.param, colors.end, a->fin_idle_timeout);
    }
    if (a->fin_hard_timeout) {
        ds_put_format(s, "%shard_timeout=%s%"PRIu16",",
                      colors.param, colors.end, a->fin_hard_timeout);
    }
    ds_chomp(s, ',');
    ds_put_format(s, "%s)%s", colors.paren, colors.end);
}
