ExprResolveKeySym(struct xkb_context *ctx, const ExprDef *expr,
                  xkb_keysym_t *sym_rtrn)
{
    int val;

    if (expr->expr.op == EXPR_IDENT) {
        const char *str = xkb_atom_text(ctx, expr->ident.ident);
        *sym_rtrn = xkb_keysym_from_name(str, 0);
        if (*sym_rtrn != XKB_KEY_NoSymbol)
            return true;
    }

    if (!ExprResolveInteger(ctx, expr, &val))
        return false;

    if (val < 0 || val >= 10)
        return false;

    *sym_rtrn = XKB_KEY_0 + (xkb_keysym_t) val;
    return true;
}
