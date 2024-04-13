do_include(struct xkb_compose_table *table, struct scanner *s,
           const char *path, unsigned include_depth)
{
    FILE *file;
    bool ok;
    char *string;
    size_t size;
    struct scanner new_s;

    if (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(s, "maximum include depth (%d) exceeded; maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        return false;
    }

    file = fopen(path, "r");
    if (!file) {
        scanner_err(s, "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        return false;
    }

    ok = map_file(file, &string, &size);
    if (!ok) {
        scanner_err(s, "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        goto err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    if (!ok)
        goto err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    fclose(file);
    return ok;
}
