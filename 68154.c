ptaaCreate(l_int32  n)
{
PTAA  *ptaa;

    PROCNAME("ptaaCreate");

    if (n <= 0)
        n = INITIAL_PTR_ARRAYSIZE;

    if ((ptaa = (PTAA *)LEPT_CALLOC(1, sizeof(PTAA))) == NULL)
        return (PTAA *)ERROR_PTR("ptaa not made", procName, NULL);
    ptaa->n = 0;
    ptaa->nalloc = n;
    if ((ptaa->pta = (PTA **)LEPT_CALLOC(n, sizeof(PTA *))) == NULL) {
        ptaaDestroy(&ptaa);
        return (PTAA *)ERROR_PTR("pta ptrs not made", procName, NULL);
    }
    return ptaa;
}
