VarCreate(ExprDef *name, ExprDef *value)
{
    VarDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_VAR;
    def->common.next = NULL;
    def->name = name;
    def->value = value;

    return def;
}
