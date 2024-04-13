selaGetSel(SELA    *sela,
           l_int32  i)
{
    PROCNAME("selaGetSel");

    if (!sela)
        return (SEL *)ERROR_PTR("sela not defined", procName, NULL);

    if (i < 0 || i >= sela->n)
        return (SEL *)ERROR_PTR("invalid index", procName, NULL);
    return sela->sel[i];
}
