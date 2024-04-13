HandleLedNameDef(KeyNamesInfo *info, LedNameDef *def,
                 enum merge_mode merge)
{
    LedNameInfo ledi;
    xkb_atom_t name;

    if (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->errorCount++;
        log_err(info->ctx,
                "Illegal indicator index (%d) specified; must be between 1 .. %d; "
                "Ignored\n", def->ndx, XKB_MAX_LEDS);
        return false;
    }

    if (!ExprResolveString(info->ctx, def->name, &name)) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%u", def->ndx);
        info->errorCount++;
        return ReportBadType(info->ctx, "indicator", "name", buf, "string");
    }

    ledi.merge = merge;
    ledi.name = name;
    return AddLedName(info, merge, true, &ledi, def->ndx - 1);
}
