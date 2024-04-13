HandleKeyNameVar(KeyNamesInfo *info, VarDef *stmt)
{
    const char *elem, *field;
    ExprDef *arrayNdx;

    if (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        return false;

    if (elem) {
        log_err(info->ctx, "Unknown element %s encountered; "
                "Default for field %s ignored\n", elem, field);
        return false;
    }

    if (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, "Unknown field encountered; "
                "Assignment to field %s ignored\n", field);
        return false;
    }

    /* We ignore explicit min/max statements, we always use computed. */
    return true;
}
