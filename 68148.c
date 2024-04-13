ptaSetPt(PTA       *pta,
         l_int32    index,
         l_float32  x,
         l_float32  y)
{
    PROCNAME("ptaSetPt");

    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);
    if (index < 0 || index >= pta->n)
        return ERROR_INT("invalid index", procName, 1);

    pta->x[index] = x;
    pta->y[index] = y;
    return 0;
}
