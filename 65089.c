GroupCompatCreate(unsigned group, ExprDef *val)
{
    GroupCompatDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_GROUP_COMPAT;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->group = group;
    def->def = val;

    return def;
}
