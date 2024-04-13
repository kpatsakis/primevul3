next_matching_table(const struct ofproto *ofproto,
                    const struct oftable *table, uint8_t table_id)
{
    return (table_id == 0xff
            ? next_visible_table(ofproto, (table - ofproto->tables) + 1)
            : NULL);
}
