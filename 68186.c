selaAddSel(SELA        *sela,
           SEL         *sel,
           const char  *selname,
           l_int32      copyflag)
{
l_int32  n;
SEL     *csel;

    PROCNAME("selaAddSel");

    if (!sela)
        return ERROR_INT("sela not defined", procName, 1);
    if (!sel)
        return ERROR_INT("sel not defined", procName, 1);
    if (!sel->name && !selname)
        return ERROR_INT("added sel must have name", procName, 1);
    if (copyflag != L_INSERT && copyflag != L_COPY)
        return ERROR_INT("invalid copyflag", procName, 1);

    if (copyflag == L_COPY) {
        if ((csel = selCopy(sel)) == NULL)
            return ERROR_INT("csel not made", procName, 1);
    } else {  /* copyflag == L_INSERT */
        csel = sel;
    }
    if (!csel->name)
        csel->name = stringNew(selname);

    n = selaGetCount(sela);
    if (n >= sela->nalloc)
        selaExtendArray(sela);
    sela->sel[n] = csel;
    sela->n++;

    return 0;
}
