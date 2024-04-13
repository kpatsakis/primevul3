ExprCreateMultiKeysymList(ExprDef *expr)
{
    unsigned nLevels = darray_size(expr->keysym_list.symsMapIndex);

    darray_resize(expr->keysym_list.symsMapIndex, 1);
    darray_resize(expr->keysym_list.symsNumEntries, 1);
    darray_item(expr->keysym_list.symsMapIndex, 0) = 0;
    darray_item(expr->keysym_list.symsNumEntries, 0) = nLevels;

    return expr;
}
