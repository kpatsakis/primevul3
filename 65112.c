parse_string(struct xkb_compose_table *table, const char *string, size_t len,
             const char *file_name)
{
    struct scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, NULL);
    if (!parse(table, &s, 0))
        return false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    return true;
}
