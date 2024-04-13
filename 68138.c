ptaCreate(l_int32  n)
{
PTA  *pta;

    PROCNAME("ptaCreate");

    if (n <= 0)
        n = INITIAL_PTR_ARRAYSIZE;

    pta = (PTA *)LEPT_CALLOC(1, sizeof(PTA));
    pta->n = 0;
    pta->nalloc = n;
    ptaChangeRefcount(pta, 1);  /* sets to 1 */

    pta->x = (l_float32 *)LEPT_CALLOC(n, sizeof(l_float32));
    pta->y = (l_float32 *)LEPT_CALLOC(n, sizeof(l_float32));
    if (!pta->x || !pta->y) {
        ptaDestroy(&pta);
        return (PTA *)ERROR_PTR("x and y arrays not both made", procName, NULL);
    }

    return pta;
}
