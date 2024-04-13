FreeXkbFile(XkbFile *file)
{
    XkbFile *next;

    while (file)
    {
        next = (XkbFile *) file->common.next;

        switch (file->file_type) {
        case FILE_TYPE_KEYMAP:
            FreeXkbFile((XkbFile *) file->defs);
            break;

        case FILE_TYPE_TYPES:
        case FILE_TYPE_COMPAT:
        case FILE_TYPE_SYMBOLS:
        case FILE_TYPE_KEYCODES:
        case FILE_TYPE_GEOMETRY:
            FreeStmt(file->defs);
            break;

        default:
            break;
        }

        free(file->name);
        free(file);
        file = next;
    }
}
