selCreateFromPta(PTA         *pta,
                 l_int32      cy,
                 l_int32      cx,
                 const char  *name)
{
l_int32  i, n, x, y, w, h;
BOX     *box;
SEL     *sel;

    PROCNAME("selCreateFromPta");

    if (!pta)
        return (SEL *)ERROR_PTR("pta not defined", procName, NULL);
    if (cy < 0 || cx < 0)
        return (SEL *)ERROR_PTR("(cy, cx) not both >= 0", procName, NULL);
    n = ptaGetCount(pta);
    if (n == 0)
        return (SEL *)ERROR_PTR("no pts in pta", procName, NULL);

    box = ptaGetBoundingRegion(pta);
    boxGetGeometry(box, &x, &y, &w, &h);
    boxDestroy(&box);
    if (x < 0 || y < 0)
        return (SEL *)ERROR_PTR("not all x and y >= 0", procName, NULL);

    sel = selCreate(y + h, x + w, name);
    selSetOrigin(sel, cy, cx);
    for (i = 0; i < n; i++) {
        ptaGetIPt(pta, i, &x, &y);
        selSetElement(sel, y, x, SEL_HIT);
    }

    return sel;
}
