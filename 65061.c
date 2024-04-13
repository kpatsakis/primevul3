HandleAliasDef(KeyNamesInfo *info, KeyAliasDef *def, enum merge_mode merge)
{
    AliasInfo *old, new;

    darray_foreach(old, info->aliases) {
        if (old->alias == def->alias) {
            if (def->real == old->real) {
                log_vrb(info->ctx, 1,
                        "Alias of %s for %s declared more than once; "
                        "First definition ignored\n",
                        KeyNameText(info->ctx, def->alias),
                        KeyNameText(info->ctx, def->real));
            }
            else {
                xkb_atom_t use, ignore;

                use = (merge == MERGE_AUGMENT ? old->real : def->real);
                ignore = (merge == MERGE_AUGMENT ? def->real : old->real);

                log_warn(info->ctx,
                         "Multiple definitions for alias %s; "
                         "Using %s, ignoring %s\n",
                         KeyNameText(info->ctx, old->alias),
                         KeyNameText(info->ctx, use),
                         KeyNameText(info->ctx, ignore));

                old->real = use;
            }

            old->merge = merge;
            return true;
        }
    }

    InitAliasInfo(&new, merge, def->alias, def->real);
    darray_append(info->aliases, new);
    return true;
}
