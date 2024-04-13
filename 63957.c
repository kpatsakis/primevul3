format_DEC_TTL(const struct ofpact_cnt_ids *a, struct ds *s)
{
    size_t i;

    ds_put_format(s, "%sdec_ttl%s", colors.paren, colors.end);
    if (a->ofpact.raw == NXAST_RAW_DEC_TTL_CNT_IDS) {
        ds_put_format(s, "%s(%s", colors.paren, colors.end);
        for (i = 0; i < a->n_controllers; i++) {
            if (i) {
                ds_put_cstr(s, ",");
            }
            ds_put_format(s, "%"PRIu16, a->cnt_ids[i]);
        }
        ds_put_format(s, "%s)%s", colors.paren, colors.end);
    }
}
