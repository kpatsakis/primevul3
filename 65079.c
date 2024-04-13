ExprCreateIdent(xkb_atom_t ident)
{
    EXPR_CREATE(ExprIdent, expr, EXPR_IDENT, EXPR_TYPE_UNKNOWN);
    expr->ident.ident = ident;
    return expr;
}
