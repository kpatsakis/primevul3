BoolVarCreate(xkb_atom_t ident, bool set)
{
    ExprDef *name, *value;
    VarDef *def;
    if (!(name = ExprCreateIdent(ident))) {
        return NULL;
    }
    if (!(value = ExprCreateBoolean(set))) {
        FreeStmt((ParseCommon *) name);
        return NULL;
    }
    if (!(def = VarCreate(name, value))) {
        FreeStmt((ParseCommon *) name);
        FreeStmt((ParseCommon *) value);
        return NULL;
    }
    return def;
}
