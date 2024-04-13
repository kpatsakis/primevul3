selaGetBrickName(SELA    *sela,
                 l_int32  hsize,
                 l_int32  vsize)
{
l_int32  i, nsels, sx, sy;
SEL     *sel;

    PROCNAME("selaGetBrickName");

    if (!sela)
        return (char *)ERROR_PTR("sela not defined", procName, NULL);

    nsels = selaGetCount(sela);
    for (i = 0; i < nsels; i++) {
        sel = selaGetSel(sela, i);
        selGetParameters(sel, &sy, &sx, NULL, NULL);
        if (hsize == sx && vsize == sy)
            return stringNew(selGetName(sel));
    }

    return (char *)ERROR_PTR("sel not found", procName, NULL);
}
