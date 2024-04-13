ptaGetArrays(PTA    *pta,
             NUMA  **pnax,
             NUMA  **pnay)
{
l_int32  i, n;
NUMA    *nax, *nay;

    PROCNAME("ptaGetArrays");

    if (!pnax && !pnay)
        return ERROR_INT("no output requested", procName, 1);
    if (pnax) *pnax = NULL;
    if (pnay) *pnay = NULL;
    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);
    if ((n = ptaGetCount(pta)) == 0)
        return ERROR_INT("pta is empty", procName, 1);

    if (pnax) {
        if ((nax = numaCreate(n)) == NULL)
            return ERROR_INT("nax not made", procName, 1);
        *pnax = nax;
        for (i = 0; i < n; i++)
            nax->array[i] = pta->x[i];
        nax->n = n;
    }
    if (pnay) {
        if ((nay = numaCreate(n)) == NULL)
            return ERROR_INT("nay not made", procName, 1);
        *pnay = nay;
        for (i = 0; i < n; i++)
            nay->array[i] = pta->y[i];
        nay->n = n;
    }
    return 0;
}
