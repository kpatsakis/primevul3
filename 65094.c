KeycodeCreate(xkb_atom_t name, int64_t value)
{
    KeycodeDef *def = malloc(sizeof(*def));
    if (!def)
        return NULL;

    def->common.type = STMT_KEYCODE;
    def->common.next = NULL;
    def->name = name;
    def->value = value;

    return def;
}
