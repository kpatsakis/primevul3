ExprCreateFieldRef(xkb_atom_t element, xkb_atom_t field)
{
    EXPR_CREATE(ExprFieldRef, expr, EXPR_FIELD_REF, EXPR_TYPE_UNKNOWN);
    expr->field_ref.element = element;
    expr->field_ref.field = field;
    return expr;
}
