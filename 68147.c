ptaRemovePt(PTA     *pta,
            l_int32  index)
{
l_int32  i, n;

    PROCNAME("ptaRemovePt");

    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);
    n = ptaGetCount(pta);
    if (index < 0 || index >= n)
        return ERROR_INT("index not in {0...n - 1}", procName, 1);

        /* Remove the point */
    for (i = index + 1; i < n; i++) {
        pta->x[i - 1] = pta->x[i];
        pta->y[i - 1] = pta->y[i];
    }
    pta->n--;
    return 0;
}
