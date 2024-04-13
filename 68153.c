ptaaAddPta(PTAA    *ptaa,
           PTA     *pta,
           l_int32  copyflag)
{
l_int32  n;
PTA     *ptac;

    PROCNAME("ptaaAddPta");

    if (!ptaa)
        return ERROR_INT("ptaa not defined", procName, 1);
    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);

    if (copyflag == L_INSERT) {
        ptac = pta;
    } else if (copyflag == L_COPY) {
        if ((ptac = ptaCopy(pta)) == NULL)
            return ERROR_INT("ptac not made", procName, 1);
    } else if (copyflag == L_CLONE) {
        if ((ptac = ptaClone(pta)) == NULL)
            return ERROR_INT("pta clone not made", procName, 1);
    } else {
        return ERROR_INT("invalid copyflag", procName, 1);
    }

    n = ptaaGetCount(ptaa);
    if (n >= ptaa->nalloc)
        ptaaExtendArray(ptaa);
    ptaa->pta[n] = ptac;
    ptaa->n++;

    return 0;
}
