ptaaDestroy(PTAA  **pptaa)
{
l_int32  i;
PTAA    *ptaa;

    PROCNAME("ptaaDestroy");

    if (pptaa == NULL) {
        L_WARNING("ptr address is NULL!\n", procName);
        return;
    }

    if ((ptaa = *pptaa) == NULL)
        return;

    for (i = 0; i < ptaa->n; i++)
        ptaDestroy(&ptaa->pta[i]);
    LEPT_FREE(ptaa->pta);

    LEPT_FREE(ptaa);
    *pptaa = NULL;
    return;
}
