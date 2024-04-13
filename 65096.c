LedNameCreate(unsigned ndx, ExprDef *name, bool virtual)
{
    LedNameDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_LED_NAME;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->ndx = ndx;
    def->name = name;
    def->virtual = virtual;

    return def;
}
