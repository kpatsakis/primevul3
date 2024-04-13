next_visible_table(const struct ofproto *ofproto, uint8_t table_id)
{
    struct oftable *table;

    for (table = &ofproto->tables[table_id];
         table < &ofproto->tables[ofproto->n_tables];
         table++) {
        if (!(table->flags & OFTABLE_HIDDEN)) {
            return table;
        }
    }

    return NULL;
}
