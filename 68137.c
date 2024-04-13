ptaCopyRange(PTA     *ptas,
             l_int32  istart,
             l_int32  iend)
{
l_int32  n, i, x, y;
PTA     *ptad;

    PROCNAME("ptaCopyRange");

    if (!ptas)
        return (PTA *)ERROR_PTR("ptas not defined", procName, NULL);
    n = ptaGetCount(ptas);
    if (istart < 0)
        istart = 0;
    if (istart >= n)
        return (PTA *)ERROR_PTR("istart out of bounds", procName, NULL);
    if (iend <= 0 || iend >= n)
        iend = n - 1;
    if (istart > iend)
        return (PTA *)ERROR_PTR("istart > iend; no pts", procName, NULL);

    if ((ptad = ptaCreate(iend - istart + 1)) == NULL)
        return (PTA *)ERROR_PTR("ptad not made", procName, NULL);
    for (i = istart; i <= iend; i++) {
        ptaGetIPt(ptas, i, &x, &y);
        ptaAddPt(ptad, x, y);
    }

    return ptad;
}
