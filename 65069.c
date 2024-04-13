MergeIncludedKeycodes(KeyNamesInfo *into, KeyNamesInfo *from,
                      enum merge_mode merge)
{
    if (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        return;
    }

    if (into->name == NULL) {
        into->name = from->name;
        from->name = NULL;
    }

    /* Merge key names. */
    if (darray_empty(into->key_names)) {
        into->key_names = from->key_names;
        darray_init(from->key_names);
        into->min_key_code = from->min_key_code;
        into->max_key_code = from->max_key_code;
    }
    else {
        if (darray_size(into->key_names) < darray_size(from->key_names))
            darray_resize0(into->key_names, darray_size(from->key_names));

        for (unsigned i = from->min_key_code; i <= from->max_key_code; i++) {
            xkb_atom_t name = darray_item(from->key_names, i);
            if (name == XKB_ATOM_NONE)
                continue;

            if (!AddKeyName(into, i, name, merge, true, false))
                into->errorCount++;
        }
    }

    /* Merge key aliases. */
    if (darray_empty(into->aliases)) {
        into->aliases = from->aliases;
        darray_init(from->aliases);
    }
    else {
        AliasInfo *alias;

        darray_foreach(alias, from->aliases) {
            KeyAliasDef def;

            def.merge = (merge == MERGE_DEFAULT ? alias->merge : merge);
            def.alias = alias->alias;
            def.real = alias->real;

            if (!HandleAliasDef(into, &def, def.merge))
                into->errorCount++;
        }
    }

    /* Merge LED names. */
    if (into->num_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               sizeof(*from->led_names) * from->num_led_names);
        into->num_led_names = from->num_led_names;
        from->num_led_names = 0;
    }
    else {
        for (xkb_led_index_t idx = 0; idx < from->num_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            if (ledi->name == XKB_ATOM_NONE)
                continue;

            ledi->merge = (merge == MERGE_DEFAULT ? ledi->merge : merge);
            if (!AddLedName(into, ledi->merge, false, ledi, idx))
                into->errorCount++;
        }
    }
}
