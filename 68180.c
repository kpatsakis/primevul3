selGetTypeAtOrigin(SEL      *sel,
                   l_int32  *ptype)
{
l_int32  sx, sy, cx, cy, i, j;

    PROCNAME("selGetTypeAtOrigin");

    if (!ptype)
        return ERROR_INT("&type not defined", procName, 1);
    *ptype = SEL_DONT_CARE;  /* init */
    if (!sel)
        return ERROR_INT("sel not defined", procName, 1);

    selGetParameters(sel, &sy, &sx, &cy, &cx);
    for (i = 0; i < sy; i++) {
        for (j = 0; j < sx; j++) {
            if (i == cy && j == cx) {
                selGetElement(sel, i, j, ptype);
                return 0;
            }
        }
    }

    return ERROR_INT("sel origin not found", procName, 1);
}
