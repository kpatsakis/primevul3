expr_value_type_to_string(enum expr_value_type type)
{
    if (type >= _EXPR_TYPE_NUM_VALUES)
        return NULL;
    return expr_value_type_strings[type];
}
