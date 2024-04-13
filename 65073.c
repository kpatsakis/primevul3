ExprCreateAction(xkb_atom_t name, ExprDef *args)
{
    EXPR_CREATE(ExprAction, expr, EXPR_ACTION_DECL, EXPR_TYPE_UNKNOWN);
    expr->action.name = name;
    expr->action.args = args;
    return expr;
}
