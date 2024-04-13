AddKeyName(KeyNamesInfo *info, xkb_keycode_t kc, xkb_atom_t name,
           enum merge_mode merge, bool same_file, bool report)
{
    xkb_atom_t old_name;
    xkb_keycode_t old_kc;
    const int verbosity = xkb_context_get_log_verbosity(info->ctx);

    report = report && ((same_file && verbosity > 0) || verbosity > 7);

    if (kc >= darray_size(info->key_names))
        darray_resize0(info->key_names, kc + 1);

    info->min_key_code = MIN(info->min_key_code, kc);
    info->max_key_code = MAX(info->max_key_code, kc);

    /* There's already a key with this keycode. */
    old_name = darray_item(info->key_names, kc);
    if (old_name != XKB_ATOM_NONE) {
        const char *lname = KeyNameText(info->ctx, old_name);
        const char *kname = KeyNameText(info->ctx, name);

        if (old_name == name) {
            if (report)
                log_warn(info->ctx,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %d\" ignored\n",
                         lname, kc);
            return true;
        }
        else if (merge == MERGE_AUGMENT) {
            if (report)
                log_warn(info->ctx,
                         "Multiple names for keycode %d; "
                         "Using %s, ignoring %s\n", kc, lname, kname);
            return true;
        }
        else {
            if (report)
                log_warn(info->ctx,
                         "Multiple names for keycode %d; "
                         "Using %s, ignoring %s\n", kc, kname, lname);
            darray_item(info->key_names, kc) = XKB_ATOM_NONE;
        }
    }

    /* There's already a key with this name. */
    old_kc = FindKeyByName(info, name);
    if (old_kc != XKB_KEYCODE_INVALID && old_kc != kc) {
        const char *kname = KeyNameText(info->ctx, name);

        if (merge == MERGE_OVERRIDE) {
            darray_item(info->key_names, old_kc) = XKB_ATOM_NONE;
            if (report)
                log_warn(info->ctx,
                         "Key name %s assigned to multiple keys; "
                         "Using %d, ignoring %d\n", kname, kc, old_kc);
        }
        else {
            if (report)
                log_vrb(info->ctx, 3,
                        "Key name %s assigned to multiple keys; "
                        "Using %d, ignoring %d\n", kname, old_kc, kc);
            return true;
        }
    }

    darray_item(info->key_names, kc) = name;
    return true;
}
