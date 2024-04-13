ExprCreateUnary(enum expr_op_type op, enum expr_value_type type,
                ExprDef *child)
{
    EXPR_CREATE(ExprUnary, expr, op, type);
    expr->unary.child = child;
    return expr;
}
