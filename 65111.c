parse_file(struct xkb_compose_table *table, FILE *file, const char *file_name)
{
    bool ok;
    char *string;
    size_t size;

    ok = map_file(file, &string, &size);
    if (!ok) {
        log_err(table->ctx, "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        return false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    return ok;
}
