SymbolsCreate(xkb_atom_t keyName, VarDef *symbols)
{
    SymbolsDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_SYMBOLS;
    def->common.next = NULL;
    def->merge = MERGE_DEFAULT;
    def->keyName = keyName;
    def->symbols = symbols;

    return def;
}
