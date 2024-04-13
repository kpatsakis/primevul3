KeyAliasCreate(xkb_atom_t alias, xkb_atom_t real)
{
    KeyAliasDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_ALIAS;
    def->common.next = NULL;
    def->alias = alias;
    def->real = real;

    return def;
}
