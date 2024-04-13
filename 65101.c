XkbFileCreate(enum xkb_file_type type, char *name, ParseCommon *defs,
              enum xkb_map_flags flags)
{
    XkbFile *file;

    file = calloc(1, sizeof(*file));
    if (!file)
        return NULL;

    XkbEscapeMapName(name);
    file->file_type = type;
    file->name = name ? name : strdup("(unnamed)");
    file->defs = defs;
    file->flags = flags;

    return file;
}
