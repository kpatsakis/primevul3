selFindMaxTranslations(SEL      *sel,
                       l_int32  *pxp,
                       l_int32  *pyp,
                       l_int32  *pxn,
                       l_int32  *pyn)
{
l_int32  sx, sy, cx, cy, i, j;
l_int32  maxxp, maxyp, maxxn, maxyn;

    PROCNAME("selaFindMaxTranslations");

    if (!pxp || !pyp || !pxn || !pyn)
        return ERROR_INT("&xp (etc) defined", procName, 1);
    *pxp = *pyp = *pxn = *pyn = 0;
    if (!sel)
        return ERROR_INT("sel not defined", procName, 1);
    selGetParameters(sel, &sy, &sx, &cy, &cx);

    maxxp = maxyp = maxxn = maxyn = 0;
    for (i = 0; i < sy; i++) {
        for (j = 0; j < sx; j++) {
            if (sel->data[i][j] == 1) {
                maxxp = L_MAX(maxxp, cx - j);
                maxyp = L_MAX(maxyp, cy - i);
                maxxn = L_MAX(maxxn, j - cx);
                maxyn = L_MAX(maxyn, i - cy);
            }
        }
    }

    *pxp = maxxp;
    *pyp = maxyp;
    *pxn = maxxn;
    *pyn = maxyn;

    return 0;
}
