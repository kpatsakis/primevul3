ExprResolveEnum(struct xkb_context *ctx, const ExprDef *expr,
                unsigned int *val_rtrn, const LookupEntry *values)
{
    if (expr->expr.op != EXPR_IDENT) {
        log_err(ctx, "Found a %s where an enumerated value was expected\n",
                expr_op_type_to_string(expr->expr.op));
        return false;
    }

    if (!SimpleLookup(ctx, values, expr->ident.ident, EXPR_TYPE_INT,
                      val_rtrn)) {
        log_err(ctx, "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, expr->ident.ident));
        while (values && values->name)
        {
            log_err(ctx, "\t%s\n", values->name);
            values++;
        }
        return false;
    }

    return true;
}
