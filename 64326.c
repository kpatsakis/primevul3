ofproto_init_tables(struct ofproto *ofproto, int n_tables)
{
    struct oftable *table;

    ovs_assert(!ofproto->n_tables);
    ovs_assert(n_tables >= 1 && n_tables <= 255);

    ofproto->n_tables = n_tables;
    ofproto->tables = xmalloc(n_tables * sizeof *ofproto->tables);
    OFPROTO_FOR_EACH_TABLE (table, ofproto) {
        oftable_init(table);
    }
}
