selCreateFromSArray(SARRAY  *sa,
                    l_int32  first,
                    l_int32  last)
{
char     ch;
char    *name, *line;
l_int32  n, len, i, w, h, y, x;
SEL     *sel;

    PROCNAME("selCreateFromSArray");

    if (!sa)
        return (SEL *)ERROR_PTR("sa not defined", procName, NULL);
    n = sarrayGetCount(sa);
    if (first < 0 || first >= n || last <= first || last >= n)
        return (SEL *)ERROR_PTR("invalid range", procName, NULL);

    name = sarrayGetString(sa, first, L_NOCOPY);
    h = last - first;
    line = sarrayGetString(sa, first + 1, L_NOCOPY);
    len = strlen(line);
    if (line[0] != '"' || line[len - 1] != '"')
        return (SEL *)ERROR_PTR("invalid format", procName, NULL);
    w = len - 2;
    if ((sel = selCreate(h, w, name)) == NULL)
        return (SEL *)ERROR_PTR("sel not made", procName, NULL);
    for (i = first + 1; i <= last; i++) {
        line = sarrayGetString(sa, i, L_NOCOPY);
        y = i - first - 1;
        for (x = 0; x < w; ++x) {
            ch = line[x + 1];  /* skip the leading double-quote */
            switch (ch)
            {
                case 'X':
                    selSetOrigin(sel, y, x);  /* set origin and hit */
                case 'x':
                    selSetElement(sel, y, x, SEL_HIT);
                    break;

                case 'O':
                    selSetOrigin(sel, y, x);  /* set origin and miss */
                case 'o':
                    selSetElement(sel, y, x, SEL_MISS);
                    break;

                case 'C':
                    selSetOrigin(sel, y, x);  /* set origin and don't-care */
                case ' ':
                    selSetElement(sel, y, x, SEL_DONT_CARE);
                    break;

                default:
                    selDestroy(&sel);
                    return (SEL *)ERROR_PTR("unknown char", procName, NULL);
            }
        }
    }

    return sel;
}
