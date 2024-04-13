HandleInterpDef(CompatInfo *info, InterpDef *def, enum merge_mode merge)
{
    enum xkb_match_operation pred;
    xkb_mod_mask_t mods;
    SymInterpInfo si;

    if (!ResolveStateAndPredicate(def->match, &pred, &mods, info)) {
        log_err(info->ctx,
                "Couldn't determine matching modifiers; "
                "Symbol interpretation ignored\n");
        return false;
    }

    si = info->default_interp;
    si.merge = merge = (def->merge == MERGE_DEFAULT ? merge : def->merge);
    si.interp.sym = def->sym;
    si.interp.match = pred;
    si.interp.mods = mods;

    if (!HandleInterpBody(info, def->def, &si)) {
        info->errorCount++;
        return false;
    }

    if (!AddInterp(info, &si, true)) {
        info->errorCount++;
        return false;
    }

    return true;
}
