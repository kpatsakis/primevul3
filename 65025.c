HandleGlobalVar(CompatInfo *info, VarDef *stmt)
{
    const char *elem, *field;
    ExprDef *ndx;
    bool ret;

    if (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field, &ndx))
        ret = false;
    else if (elem && istreq(elem, "interpret"))
        ret = SetInterpField(info, &info->default_interp, field, ndx,
                             stmt->value);
    else if (elem && istreq(elem, "indicator"))
        ret = SetLedMapField(info, &info->default_led, field, ndx,
                             stmt->value);
    else
        ret = SetActionField(info->ctx, info->actions, &info->mods,
                             elem, field, ndx, stmt->value);
    return ret;
}
