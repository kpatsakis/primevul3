ptaaReplacePta(PTAA    *ptaa,
               l_int32  index,
               PTA     *pta)
{
l_int32  n;

    PROCNAME("ptaaReplacePta");

    if (!ptaa)
        return ERROR_INT("ptaa not defined", procName, 1);
    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);
    n = ptaaGetCount(ptaa);
    if (index < 0 || index >= n)
        return ERROR_INT("index not valid", procName, 1);

    ptaDestroy(&ptaa->pta[index]);
    ptaa->pta[index] = pta;
    return 0;
}
