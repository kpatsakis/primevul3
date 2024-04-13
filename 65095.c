LedMapCreate(xkb_atom_t name, VarDef *body)
{
    LedMapDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_LED_MAP;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->name = name;
    def->body = body;

    return def;
}
