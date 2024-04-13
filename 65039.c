ExprResolveButton(struct xkb_context *ctx, const ExprDef *expr, int *btn_rtrn)
{
    return ExprResolveIntegerLookup(ctx, expr, btn_rtrn, SimpleLookup,
                                    buttonNames);
}
