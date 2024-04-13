selRotateOrth(SEL     *sel,
              l_int32  quads)
{
l_int32  i, j, ni, nj, sx, sy, cx, cy, nsx, nsy, ncx, ncy, type;
SEL     *seld;

    PROCNAME("selRotateOrth");

    if (!sel)
        return (SEL *)ERROR_PTR("sel not defined", procName, NULL);
    if (quads < 0 || quads > 4)
        return (SEL *)ERROR_PTR("quads not in {0,1,2,3,4}", procName, NULL);
    if (quads == 0 || quads == 4)
        return selCopy(sel);

    selGetParameters(sel, &sy, &sx, &cy, &cx);
    if (quads == 1) {  /* 90 degrees cw */
        nsx = sy;
        nsy = sx;
        ncx = sy - cy - 1;
        ncy = cx;
    } else if (quads == 2) {  /* 180 degrees cw */
        nsx = sx;
        nsy = sy;
        ncx = sx - cx - 1;
        ncy = sy - cy - 1;
    } else {  /* 270 degrees cw */
        nsx = sy;
        nsy = sx;
        ncx = cy;
        ncy = sx - cx - 1;
    }
    seld = selCreateBrick(nsy, nsx, ncy, ncx, SEL_DONT_CARE);
    if (sel->name)
        seld->name = stringNew(sel->name);

    for (i = 0; i < sy; i++) {
        for (j = 0; j < sx; j++) {
            selGetElement(sel, i, j, &type);
            if (quads == 1) {
               ni = j;
               nj = sy - i - 1;
            } else if (quads == 2) {
               ni = sy - i - 1;
               nj = sx - j - 1;
            } else {  /* quads == 3 */
               ni = sx - j - 1;
               nj = i;
            }
            selSetElement(seld, ni, nj, type);
        }
    }

    return seld;
}
