SimpleLookup(struct xkb_context *ctx, const void *priv, xkb_atom_t field,
             enum expr_value_type type, unsigned int *val_rtrn)
{
    const LookupEntry *entry;
    const char *str;

    if (!priv || field == XKB_ATOM_NONE || type != EXPR_TYPE_INT)
        return false;

    str = xkb_atom_text(ctx, field);
    for (entry = priv; entry && entry->name; entry++) {
        if (istreq(str, entry->name)) {
            *val_rtrn = entry->value;
            return true;
        }
    }

    return false;
}
