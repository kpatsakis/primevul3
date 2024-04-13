stmt_type_to_string(enum stmt_type type)
{
    if (type >= _STMT_NUM_VALUES)
        return NULL;
    return stmt_type_strings[type];
}
