ExprCreateString(xkb_atom_t str)
{
    EXPR_CREATE(ExprString, expr, EXPR_VALUE, EXPR_TYPE_STRING);
    expr->string.str = str;
    return expr;
}
