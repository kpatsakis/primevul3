ExprCreateBinary(enum expr_op_type op, ExprDef *left, ExprDef *right)
{
    EXPR_CREATE(ExprBinary, expr, op, EXPR_TYPE_UNKNOWN);

    if (op == EXPR_ASSIGN || left->expr.value_type == EXPR_TYPE_UNKNOWN)
        expr->expr.value_type = right->expr.value_type;
    else if (left->expr.value_type == right->expr.value_type ||
             right->expr.value_type == EXPR_TYPE_UNKNOWN)
        expr->expr.value_type = left->expr.value_type;
    expr->binary.left = left;
    expr->binary.right = right;

    return expr;
}
