XkbFileFromComponents(struct xkb_context *ctx,
                      const struct xkb_component_names *kkctgs)
{
    char *const components[] = {
        kkctgs->keycodes, kkctgs->types,
        kkctgs->compat, kkctgs->symbols,
    };
    enum xkb_file_type type;
    IncludeStmt *include = NULL;
    XkbFile *file = NULL;
    ParseCommon *defs = NULL;

    for (type = FIRST_KEYMAP_FILE_TYPE; type <= LAST_KEYMAP_FILE_TYPE; type++) {
        include = IncludeCreate(ctx, components[type], MERGE_DEFAULT);
        if (!include)
            goto err;

        file = XkbFileCreate(type, NULL, (ParseCommon *) include, 0);
        if (!file) {
            FreeInclude(include);
            goto err;
        }

        defs = AppendStmt(defs, &file->common);
    }

    file = XkbFileCreate(FILE_TYPE_KEYMAP, NULL, defs, 0);
    if (!file)
        goto err;

    return file;

err:
    FreeXkbFile((XkbFile *) defs);
    return NULL;
}
