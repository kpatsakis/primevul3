selGetName(SEL  *sel)
{
    PROCNAME("selGetName");

    if (!sel)
        return (char *)ERROR_PTR("sel not defined", procName, NULL);

    return sel->name;
}
