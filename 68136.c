ptaCopy(PTA  *pta)
{
l_int32    i;
l_float32  x, y;
PTA       *npta;

    PROCNAME("ptaCopy");

    if (!pta)
        return (PTA *)ERROR_PTR("pta not defined", procName, NULL);

    if ((npta = ptaCreate(pta->nalloc)) == NULL)
        return (PTA *)ERROR_PTR("npta not made", procName, NULL);

    for (i = 0; i < pta->n; i++) {
        ptaGetPt(pta, i, &x, &y);
        ptaAddPt(npta, x, y);
    }

    return npta;
}
