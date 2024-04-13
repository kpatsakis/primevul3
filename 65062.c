HandleIncludeKeycodes(KeyNamesInfo *info, IncludeStmt *include)
{
    KeyNamesInfo included;

    InitKeyNamesInfo(&included, info->ctx);
    included.name = include->stmt;
    include->stmt = NULL;

    for (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        KeyNamesInfo next_incl;
        XkbFile *file;

        file = ProcessIncludeFile(info->ctx, stmt, FILE_TYPE_KEYCODES);
        if (!file) {
            info->errorCount += 10;
            ClearKeyNamesInfo(&included);
            return false;
        }

        InitKeyNamesInfo(&next_incl, info->ctx);

        HandleKeycodesFile(&next_incl, file, MERGE_OVERRIDE);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge);

        ClearKeyNamesInfo(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge);
    ClearKeyNamesInfo(&included);

    return (info->errorCount == 0);
}
