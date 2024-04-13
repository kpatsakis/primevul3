query_tables_desc(struct ofproto *ofproto, struct ofputil_table_desc **descp)
{
    struct ofputil_table_desc *table_desc;
    size_t i;

    table_desc = *descp = xcalloc(ofproto->n_tables, sizeof *table_desc);
    for (i = 0; i < ofproto->n_tables; i++) {
        struct ofputil_table_desc *td = &table_desc[i];
        query_table_desc__(td, ofproto, i);
    }
}
