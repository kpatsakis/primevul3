add_node(struct xkb_compose_table *table, xkb_keysym_t keysym)
{
    struct compose_node new = {
        .keysym = keysym,
        .next = 0,
        .is_leaf = true,
    };
    darray_append(table->nodes, new);
    return darray_size(table->nodes) - 1;
}
