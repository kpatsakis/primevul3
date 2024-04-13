ExprResolveGroup(struct xkb_context *ctx, const ExprDef *expr,
                 xkb_layout_index_t *group_rtrn)
{
    bool ok;
    int result;

    ok = ExprResolveIntegerLookup(ctx, expr, &result, SimpleLookup,
                                  groupNames);
    if (!ok)
        return false;

    if (result <= 0 || result > XKB_MAX_GROUPS) {
        log_err(ctx, "Group index %u is out of range (1..%d)\n",
                result, XKB_MAX_GROUPS);
        return false;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    return true;
}
