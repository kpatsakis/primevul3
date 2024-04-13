ModMapCreate(xkb_atom_t modifier, ExprDef *keys)
{
    ModMapDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_MODMAP;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->modifier = modifier;
    def->keys = keys;

    return def;
}
