SetInterpField(CompatInfo *info, SymInterpInfo *si, const char *field,
               ExprDef *arrayNdx, ExprDef *value)
{
    xkb_mod_index_t ndx;

    if (istreq(field, "action")) {
        if (arrayNdx)
            return ReportSINotArray(info, si, field);

        if (!HandleActionDef(info->ctx, info->actions, &info->mods,
                             value, &si->interp.action))
            return false;

        si->defined |= SI_FIELD_ACTION;
    }
    else if (istreq(field, "virtualmodifier") ||
             istreq(field, "virtualmod")) {
        if (arrayNdx)
            return ReportSINotArray(info, si, field);

        if (!ExprResolveMod(info->ctx, value, MOD_VIRT, &info->mods, &ndx))
            return ReportSIBadType(info, si, field, "virtual modifier");

        si->interp.virtual_mod = ndx;
        si->defined |= SI_FIELD_VIRTUAL_MOD;
    }
    else if (istreq(field, "repeat")) {
        bool set;

        if (arrayNdx)
            return ReportSINotArray(info, si, field);

        if (!ExprResolveBoolean(info->ctx, value, &set))
            return ReportSIBadType(info, si, field, "boolean");

        si->interp.repeat = set;

        si->defined |= SI_FIELD_AUTO_REPEAT;
    }
    else if (istreq(field, "locking")) {
        log_dbg(info->ctx,
                "The \"locking\" field in symbol interpretation is unsupported; "
                "Ignored\n");
    }
    else if (istreq(field, "usemodmap") ||
             istreq(field, "usemodmapmods")) {
        unsigned int val;

        if (arrayNdx)
            return ReportSINotArray(info, si, field);

        if (!ExprResolveEnum(info->ctx, value, &val, useModMapValueNames))
            return ReportSIBadType(info, si, field, "level specification");

        si->interp.level_one_only = val;
        si->defined |= SI_FIELD_LEVEL_ONE_ONLY;
    }
    else {
        return ReportBadField(info->ctx, "symbol interpretation", field,
                              siText(si, info));
    }

    return true;
}
