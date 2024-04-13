ExprCreateKeyName(xkb_atom_t key_name)
{
    EXPR_CREATE(ExprKeyName, expr, EXPR_VALUE, EXPR_TYPE_KEYNAME);
    expr->key_name.key_name = key_name;
    return expr;
}
