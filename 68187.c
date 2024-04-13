selaCreate(l_int32  n)
{
SELA  *sela;

    PROCNAME("selaCreate");

    if (n <= 0)
        n = INITIAL_PTR_ARRAYSIZE;
    if (n > MANY_SELS)
        L_WARNING("%d sels\n", procName, n);

    if ((sela = (SELA *)LEPT_CALLOC(1, sizeof(SELA))) == NULL)
        return (SELA *)ERROR_PTR("sela not made", procName, NULL);

    sela->nalloc = n;
    sela->n = 0;

        /* make array of se ptrs */
    if ((sela->sel = (SEL **)LEPT_CALLOC(n, sizeof(SEL *))) == NULL) {
        LEPT_FREE(sela);
        return (SELA *)ERROR_PTR("sel ptrs not made", procName, NULL);
    }
    return sela;
}
