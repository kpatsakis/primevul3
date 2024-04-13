ExprCreateKeysymList(xkb_keysym_t sym)
{
    EXPR_CREATE(ExprKeysymList, expr, EXPR_KEYSYM_LIST, EXPR_TYPE_SYMBOLS);

    darray_init(expr->keysym_list.syms);
    darray_init(expr->keysym_list.symsMapIndex);
    darray_init(expr->keysym_list.symsNumEntries);

    darray_append(expr->keysym_list.syms, sym);
    darray_append(expr->keysym_list.symsMapIndex, 0);
    darray_append(expr->keysym_list.symsNumEntries, 1);

    return expr;
}
