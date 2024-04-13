ptaGetIPt(PTA      *pta,
          l_int32   index,
          l_int32  *px,
          l_int32  *py)
{
    PROCNAME("ptaGetIPt");

    if (px) *px = 0;
    if (py) *py = 0;
    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);
    if (index < 0 || index >= pta->n)
        return ERROR_INT("invalid index", procName, 1);

    if (px) *px = (l_int32)(pta->x[index] + 0.5);
    if (py) *py = (l_int32)(pta->y[index] + 0.5);
    return 0;
}
