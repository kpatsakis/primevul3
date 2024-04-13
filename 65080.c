ExprCreateInteger(int ival)
{
    EXPR_CREATE(ExprInteger, expr, EXPR_VALUE, EXPR_TYPE_INT);
    expr->integer.ival = ival;
    return expr;
}
