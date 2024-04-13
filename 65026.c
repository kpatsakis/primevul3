HandleInterpBody(CompatInfo *info, VarDef *def, SymInterpInfo *si)
{
    bool ok = true;
    const char *elem, *field;
    ExprDef *arrayNdx;

    for (; def; def = (VarDef *) def->common.next) {
        if (def->name && def->name->expr.op == EXPR_FIELD_REF) {
            log_err(info->ctx,
                    "Cannot set a global default value from within an interpret statement; "
                    "Move statements to the global file scope\n");
            ok = false;
            continue;
        }

        ok = ExprResolveLhs(info->ctx, def->name, &elem, &field, &arrayNdx);
        if (!ok)
            continue;

        ok = SetInterpField(info, si, field, arrayNdx, def->value);
    }

    return ok;
}
