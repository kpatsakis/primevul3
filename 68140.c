ptaGetCount(PTA  *pta)
{
    PROCNAME("ptaGetCount");

    if (!pta)
        return ERROR_INT("pta not defined", procName, 0);

    return pta->n;
}
