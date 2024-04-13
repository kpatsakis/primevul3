ExprCreateBoolean(bool set)
{
    EXPR_CREATE(ExprBoolean, expr, EXPR_VALUE, EXPR_TYPE_BOOLEAN);
    expr->boolean.set = set;
    return expr;
}
