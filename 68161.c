ptaaTruncate(PTAA  *ptaa)
{
l_int32  i, n, np;
PTA     *pta;

    PROCNAME("ptaaTruncate");

    if (!ptaa)
        return ERROR_INT("ptaa not defined", procName, 1);

    n = ptaaGetCount(ptaa);
    for (i = n - 1; i >= 0; i--) {
        pta = ptaaGetPta(ptaa, i, L_CLONE);
        if (!pta) {
            ptaa->n--;
            continue;
        }
        np = ptaGetCount(pta);
        ptaDestroy(&pta);
        if (np == 0) {
            ptaDestroy(&ptaa->pta[i]);
            ptaa->n--;
        } else {
            break;
        }
    }
    return 0;
}
