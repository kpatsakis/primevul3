expr_op_type_to_string(enum expr_op_type type)
{
    if (type >= _EXPR_NUM_VALUES)
        return NULL;
    return expr_op_type_strings[type];
}
