selSetOrigin(SEL     *sel,
             l_int32  cy,
             l_int32  cx)
{
    PROCNAME("selSetOrigin");

    if (!sel)
        return ERROR_INT("sel not defined", procName, 1);
    sel->cy = cy;
    sel->cx = cx;
    return 0;
}
