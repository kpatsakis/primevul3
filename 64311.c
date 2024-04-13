ofproto_get_n_visible_tables(const struct ofproto *ofproto)
{
    uint8_t n = ofproto->n_tables;

    /* Count only non-hidden tables in the number of tables.  (Hidden tables,
     * if present, are always at the end.) */
    while(n && (ofproto->tables[n - 1].flags & OFTABLE_HIDDEN)) {
        n--;
    }

    return n;
}
