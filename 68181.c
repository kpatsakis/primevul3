selPrintToString(SEL  *sel)
{
char     is_center;
char    *str, *strptr;
l_int32  type;
l_int32  sx, sy, cx, cy, x, y;

    PROCNAME("selPrintToString");

    if (!sel)
        return (char *)ERROR_PTR("sel not defined", procName, NULL);

    selGetParameters(sel, &sy, &sx, &cy, &cx);
    if ((str = (char *)LEPT_CALLOC(1, sy * (sx + 1) + 1)) == NULL)
        return (char *)ERROR_PTR("calloc fail for str", procName, NULL);
    strptr = str;

    for (y = 0; y < sy; ++y) {
        for (x = 0; x < sx; ++x) {
            selGetElement(sel, y, x, &type);
            is_center = (x == cx && y == cy);
            switch (type) {
                case SEL_HIT:
                    *(strptr++) = is_center ? 'X' : 'x';
                    break;
                case SEL_MISS:
                    *(strptr++) = is_center ? 'O' : 'o';
                    break;
                case SEL_DONT_CARE:
                    *(strptr++) = is_center ? 'C' : ' ';
                    break;
            }
        }
        *(strptr++) = '\n';
    }

    return str;
}
