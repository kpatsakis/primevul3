ptaInsertPt(PTA     *pta,
            l_int32  index,
            l_int32  x,
            l_int32  y)
{
l_int32  i, n;

    PROCNAME("ptaInsertPt");

    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);
    n = ptaGetCount(pta);
    if (index < 0 || index > n)
        return ERROR_INT("index not in {0...n}", procName, 1);

    if (n > pta->nalloc)
        ptaExtendArrays(pta);
    pta->n++;
    for (i = n; i > index; i--) {
        pta->x[i] = pta->x[i - 1];
        pta->y[i] = pta->y[i - 1];
    }
    pta->x[index] = x;
    pta->y[index] = y;
    return 0;
}
