HandleLedMapDef(CompatInfo *info, LedMapDef *def, enum merge_mode merge)
{
    LedInfo ledi;
    VarDef *var;
    bool ok;

    if (def->merge != MERGE_DEFAULT)
        merge = def->merge;

    ledi = info->default_led;
    ledi.merge = merge;
    ledi.led.name = def->name;

    ok = true;
    for (var = def->body; var != NULL; var = (VarDef *) var->common.next) {
        const char *elem, *field;
        ExprDef *arrayNdx;
        if (!ExprResolveLhs(info->ctx, var->name, &elem, &field, &arrayNdx)) {
            ok = false;
            continue;
        }

        if (elem) {
            log_err(info->ctx,
                    "Cannot set defaults for \"%s\" element in indicator map; "
                    "Assignment to %s.%s ignored\n", elem, elem, field);
            ok = false;
        }
        else {
            ok = SetLedMapField(info, &ledi, field, arrayNdx, var->value) && ok;
        }
    }

    if (ok)
        return AddLedMap(info, &ledi, true);

    return false;
}
