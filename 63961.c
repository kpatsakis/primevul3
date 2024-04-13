format_GOTO_TABLE(const struct ofpact_goto_table *a, struct ds *s)
{
    ds_put_format(s, "%sgoto_table:%s%"PRIu8,
                  colors.param, colors.end, a->table_id);
}
