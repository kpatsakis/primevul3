ptaChangeRefcount(PTA     *pta,
                  l_int32  delta)
{
    PROCNAME("ptaChangeRefcount");

    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);
    pta->refcount += delta;
    return 0;
}
