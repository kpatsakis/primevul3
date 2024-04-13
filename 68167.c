selCopy(SEL  *sel)
{
l_int32  sx, sy, cx, cy, i, j;
SEL     *csel;

    PROCNAME("selCopy");

    if (!sel)
        return (SEL *)ERROR_PTR("sel not defined", procName, NULL);

    if ((csel = (SEL *)LEPT_CALLOC(1, sizeof(SEL))) == NULL)
        return (SEL *)ERROR_PTR("csel not made", procName, NULL);
    selGetParameters(sel, &sy, &sx, &cy, &cx);
    csel->sy = sy;
    csel->sx = sx;
    csel->cy = cy;
    csel->cx = cx;

    if ((csel->data = create2dIntArray(sy, sx)) == NULL) {
        LEPT_FREE(csel);
        return (SEL *)ERROR_PTR("sel data not made", procName, NULL);
    }

    for (i = 0; i < sy; i++)
        for (j = 0; j < sx; j++)
            csel->data[i][j] = sel->data[i][j];

    if (sel->name)
        csel->name = stringNew(sel->name);

    return csel;
}
