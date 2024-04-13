ExprAppendKeysymList(ExprDef *expr, xkb_keysym_t sym)
{
    unsigned nSyms = darray_size(expr->keysym_list.syms);

    darray_append(expr->keysym_list.symsMapIndex, nSyms);
    darray_append(expr->keysym_list.symsNumEntries, 1);
    darray_append(expr->keysym_list.syms, sym);

    return expr;
}
