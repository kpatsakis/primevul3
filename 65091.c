InterpCreate(xkb_keysym_t sym, ExprDef *match)
{
    InterpDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_INTERP;
    def->common.next = NULL;
    def->sym = sym;
    def->match = match;
    def->def = NULL;

    return def;
}
