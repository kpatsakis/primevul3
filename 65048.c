ExprResolveMod(struct xkb_context *ctx, const ExprDef *def,
               enum mod_type mod_type, const struct xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    xkb_mod_index_t ndx;
    xkb_atom_t name;

    if (def->expr.op != EXPR_IDENT) {
        log_err(ctx,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                expr_op_type_to_string(def->expr.op));
        return false;
    }

    name = def->ident.ident;
    ndx = XkbModNameToIndex(mods, name, mod_type);
    if (ndx == XKB_MOD_INVALID) {
        log_err(ctx,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        return false;
    }

    *ndx_rtrn = ndx;
    return true;
}
