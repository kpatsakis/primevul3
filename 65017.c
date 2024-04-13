AddInterp(CompatInfo *info, SymInterpInfo *new, bool same_file)
{
    SymInterpInfo *old = FindMatchingInterp(info, new);
    if (old) {
        const int verbosity = xkb_context_get_log_verbosity(info->ctx);
        const bool report = (same_file && verbosity > 0) || verbosity > 9;
        enum si_field collide = 0;

        if (new->merge == MERGE_REPLACE) {
            if (report)
                log_warn(info->ctx,
                         "Multiple definitions for \"%s\"; "
                         "Earlier interpretation ignored\n",
                         siText(new, info));
            *old = *new;
            return true;
        }

        if (UseNewInterpField(SI_FIELD_VIRTUAL_MOD, old, new, report,
                              &collide)) {
            old->interp.virtual_mod = new->interp.virtual_mod;
            old->defined |= SI_FIELD_VIRTUAL_MOD;
        }
        if (UseNewInterpField(SI_FIELD_ACTION, old, new, report,
                              &collide)) {
            old->interp.action = new->interp.action;
            old->defined |= SI_FIELD_ACTION;
        }
        if (UseNewInterpField(SI_FIELD_AUTO_REPEAT, old, new, report,
                              &collide)) {
            old->interp.repeat = new->interp.repeat;
            old->defined |= SI_FIELD_AUTO_REPEAT;
        }
        if (UseNewInterpField(SI_FIELD_LEVEL_ONE_ONLY, old, new, report,
                              &collide)) {
            old->interp.level_one_only = new->interp.level_one_only;
            old->defined |= SI_FIELD_LEVEL_ONE_ONLY;
        }

        if (collide) {
            log_warn(info->ctx,
                     "Multiple interpretations of \"%s\"; "
                     "Using %s definition for duplicate fields\n",
                     siText(new, info),
                     (new->merge != MERGE_AUGMENT ? "last" : "first"));
        }

        return true;
    }

    darray_append(info->interps, *new);
    return true;
}
