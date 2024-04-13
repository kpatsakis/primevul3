ExprCreateArrayRef(xkb_atom_t element, xkb_atom_t field, ExprDef *entry)
{
    EXPR_CREATE(ExprArrayRef, expr, EXPR_ARRAY_REF, EXPR_TYPE_UNKNOWN);
    expr->array_ref.element = element;
    expr->array_ref.field = field;
    expr->array_ref.entry = entry;
    return expr;
}
