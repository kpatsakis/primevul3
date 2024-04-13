ExprResolveMask(struct xkb_context *ctx, const ExprDef *expr,
                unsigned int *mask_rtrn, const LookupEntry *values)
{
    return ExprResolveMaskLookup(ctx, expr, mask_rtrn, SimpleLookup, values);
}
