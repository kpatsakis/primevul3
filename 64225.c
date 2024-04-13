first_matching_table(const struct ofproto *ofproto, uint8_t table_id)
{
    if (table_id == 0xff) {
        return next_visible_table(ofproto, 0);
    } else if (table_id < ofproto->n_tables) {
        return &ofproto->tables[table_id];
    } else {
        return NULL;
    }
}
