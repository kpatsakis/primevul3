oftable_set_name(struct oftable *table, const char *name)
{
    if (name && name[0]) {
        int len = strnlen(name, OFP_MAX_TABLE_NAME_LEN);
        if (!table->name || strncmp(name, table->name, len)) {
            free(table->name);
            table->name = xmemdup0(name, len);
        }
    } else {
        free(table->name);
        table->name = NULL;
    }
}
