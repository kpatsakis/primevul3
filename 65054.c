AddLedName(KeyNamesInfo *info, enum merge_mode merge, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    const int verbosity = xkb_context_get_log_verbosity(info->ctx);
    const bool report = (same_file && verbosity > 0) || verbosity > 9;
    const bool replace = (merge == MERGE_REPLACE || merge == MERGE_OVERRIDE);

    /* LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    if (old) {
        if (old_idx == new_idx) {
            log_warn(info->ctx,
                     "Multiple indicators named \"%s\"; "
                     "Identical definitions ignored\n",
                     xkb_atom_text(info->ctx, new->name));
            return true;
        }

        if (report) {
            xkb_led_index_t use = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx,
                     "Multiple indicators named %s; Using %d, ignoring %d\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        if (replace)
            *old = *new;

        return true;
    }

    if (new_idx >= info->num_led_names)
        info->num_led_names = new_idx + 1;

    /* LED with the same index already exists. */
    old = &info->led_names[new_idx];
    if (old->name != XKB_ATOM_NONE) {
        if (report) {
            const xkb_atom_t use = (replace ? new->name : old->name);
            const xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, "Multiple names for indicator %d; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        if (replace)
            *old = *new;

        return true;
    }

    *old = *new;
    return true;
}
