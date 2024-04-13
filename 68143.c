ptaGetRefcount(PTA  *pta)
{
    PROCNAME("ptaGetRefcount");

    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);
    return pta->refcount;
}
