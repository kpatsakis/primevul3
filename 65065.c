HandleKeycodesFile(KeyNamesInfo *info, XkbFile *file, enum merge_mode merge)
{
    bool ok;

    free(info->name);
    info->name = strdup_safe(file->name);

    for (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        switch (stmt->type) {
        case STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt);
            break;
        case STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, merge);
            break;
        case STMT_ALIAS:
            ok = HandleAliasDef(info, (KeyAliasDef *) stmt, merge);
            break;
        case STMT_VAR:
            ok = HandleKeyNameVar(info, (VarDef *) stmt);
            break;
        case STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, merge);
            break;
        default:
            log_err(info->ctx,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            break;
        }

        if (!ok)
            info->errorCount++;

        if (info->errorCount > 10) {
            log_err(info->ctx, "Abandoning keycodes file \"%s\"\n",
                    file->name);
            break;
        }
    }
}
