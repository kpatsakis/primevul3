IncludeCreate(struct xkb_context *ctx, char *str, enum merge_mode merge)
{
    IncludeStmt *incl, *first;
    char *file, *map, *stmt, *tmp, *extra_data;
    char nextop;

    incl = first = NULL;
    file = map = NULL;
    tmp = str;
    stmt = strdup_safe(str);
    while (tmp && *tmp)
    {
        if (!ParseIncludeMap(&tmp, &file, &map, &nextop, &extra_data))
            goto err;

        /*
         * Given an RMLVO (here layout) like 'us,,fr', the rules parser
         * will give out something like 'pc+us+:2+fr:3+inet(evdev)'.
         * We should just skip the ':2' in this case and leave it to the
         * appropriate section to deal with the empty group.
         */
        if (isempty(file)) {
            free(file);
            free(map);
            free(extra_data);
            continue;
        }

        if (first == NULL) {
            first = incl = malloc(sizeof(*first));
        } else {
            incl->next_incl = malloc(sizeof(*first));
            incl = incl->next_incl;
        }

        if (!incl)
            break;

        incl->common.type = STMT_INCLUDE;
        incl->common.next = NULL;
        incl->merge = merge;
        incl->stmt = NULL;
        incl->file = file;
        incl->map = map;
        incl->modifier = extra_data;
        incl->next_incl = NULL;

        if (nextop == '|')
            merge = MERGE_AUGMENT;
        else
            merge = MERGE_OVERRIDE;
    }

    if (first)
        first->stmt = stmt;
    else
        free(stmt);

    return first;

err:
    log_err(ctx, "Illegal include statement \"%s\"; Ignored\n", stmt);
    FreeInclude(first);
    free(stmt);
    return NULL;
}
