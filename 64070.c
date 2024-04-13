parse_GOTO_TABLE(char *arg, struct ofpbuf *ofpacts,
                 enum ofputil_protocol *usable_protocols OVS_UNUSED)
{
    struct ofpact_goto_table *ogt = ofpact_put_GOTO_TABLE(ofpacts);
    char *table_s = strsep(&arg, ",");
    if (!table_s || !table_s[0]) {
        return xstrdup("instruction goto-table needs table id");
    }
    return str_to_u8(table_s, "table", &ogt->table_id);
}
