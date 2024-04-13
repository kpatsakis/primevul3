selaFindSelByName(SELA        *sela,
                  const char  *name,
                  l_int32     *pindex,
                  SEL        **psel)
{
l_int32  i, n;
char    *sname;
SEL     *sel;

    PROCNAME("selaFindSelByName");

    if (pindex) *pindex = -1;
    if (psel) *psel = NULL;

    if (!sela)
        return ERROR_INT("sela not defined", procName, 1);

    n = selaGetCount(sela);
    for (i = 0; i < n; i++)
    {
        if ((sel = selaGetSel(sela, i)) == NULL) {
            L_WARNING("missing sel\n", procName);
            continue;
        }

        sname = selGetName(sel);
        if (sname && (!strcmp(name, sname))) {
            if (pindex)
                *pindex = i;
            if (psel)
                *psel = sel;
            return 0;
        }
    }

    return 1;
}
