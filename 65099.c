VModCreate(xkb_atom_t name, ExprDef *value)
{
    VModDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_VMOD;
    def->common.next = NULL;
    def->name = name;
    def->value = value;

    return def;
}
