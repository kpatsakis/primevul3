ExprResolveInteger(struct xkb_context *ctx, const ExprDef *expr,
                   int *val_rtrn)
{
    return ExprResolveIntegerLookup(ctx, expr, val_rtrn, NULL, NULL);
}
