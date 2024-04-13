SetLedMapField(CompatInfo *info, LedInfo *ledi, const char *field,
               ExprDef *arrayNdx, ExprDef *value)
{
    bool ok = true;

    if (istreq(field, "modifiers") || istreq(field, "mods")) {
        if (arrayNdx)
            return ReportLedNotArray(info, ledi, field);

        if (!ExprResolveModMask(info->ctx, value, MOD_BOTH,
                                &info->mods, &ledi->led.mods.mods))
            return ReportLedBadType(info, ledi, field, "modifier mask");

        ledi->defined |= LED_FIELD_MODS;
    }
    else if (istreq(field, "groups")) {
        unsigned int mask;

        if (arrayNdx)
            return ReportLedNotArray(info, ledi, field);

        if (!ExprResolveMask(info->ctx, value, &mask, groupMaskNames))
            return ReportLedBadType(info, ledi, field, "group mask");

        ledi->led.groups = mask;
        ledi->defined |= LED_FIELD_GROUPS;
    }
    else if (istreq(field, "controls") || istreq(field, "ctrls")) {
        unsigned int mask;

        if (arrayNdx)
            return ReportLedNotArray(info, ledi, field);

        if (!ExprResolveMask(info->ctx, value, &mask, ctrlMaskNames))
            return ReportLedBadType(info, ledi, field, "controls mask");

        ledi->led.ctrls = mask;
        ledi->defined |= LED_FIELD_CTRLS;
    }
    else if (istreq(field, "allowexplicit")) {
        log_dbg(info->ctx,
                "The \"allowExplicit\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    else if (istreq(field, "whichmodstate") ||
             istreq(field, "whichmodifierstate")) {
        unsigned int mask;

        if (arrayNdx)
            return ReportLedNotArray(info, ledi, field);

        if (!ExprResolveMask(info->ctx, value, &mask,
                             modComponentMaskNames))
            return ReportLedBadType(info, ledi, field,
                                    "mask of modifier state components");

        ledi->led.which_mods = mask;
    }
    else if (istreq(field, "whichgroupstate")) {
        unsigned mask;

        if (arrayNdx)
            return ReportLedNotArray(info, ledi, field);

        if (!ExprResolveMask(info->ctx, value, &mask,
                             groupComponentMaskNames))
            return ReportLedBadType(info, ledi, field,
                                    "mask of group state components");

        ledi->led.which_groups = mask;
    }
    else if (istreq(field, "driveskbd") ||
             istreq(field, "driveskeyboard") ||
             istreq(field, "leddriveskbd") ||
             istreq(field, "leddriveskeyboard") ||
             istreq(field, "indicatordriveskbd") ||
             istreq(field, "indicatordriveskeyboard")) {
        log_dbg(info->ctx,
                "The \"%s\" field in indicator statements is unsupported; "
                "Ignored\n", field);
    }
    else if (istreq(field, "index")) {
        /* Users should see this, it might cause unexpected behavior. */
        log_err(info->ctx,
                "The \"index\" field in indicator statements is unsupported; "
                "Ignored\n");
    }
    else {
        log_err(info->ctx,
                "Unknown field %s in map for %s indicator; "
                "Definition ignored\n",
                field, xkb_atom_text(info->ctx, ledi->led.name));
        ok = false;
    }

    return ok;
}
