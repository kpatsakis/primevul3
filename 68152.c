ptaaAddPt(PTAA      *ptaa,
          l_int32    ipta,
          l_float32  x,
          l_float32  y)
{
PTA  *pta;

    PROCNAME("ptaaAddPt");

    if (!ptaa)
        return ERROR_INT("ptaa not defined", procName, 1);
    if (ipta < 0 || ipta >= ptaa->n)
        return ERROR_INT("index ipta not valid", procName, 1);

    pta = ptaaGetPta(ptaa, ipta, L_CLONE);
    ptaAddPt(pta, x, y);
    ptaDestroy(&pta);
    return 0;
}
