selCreateFromPix(PIX         *pix,
                 l_int32      cy,
                 l_int32      cx,
                 const char  *name)
{
SEL      *sel;
l_int32   i, j, w, h, d;
l_uint32  val;

    PROCNAME("selCreateFromPix");

    if (!pix)
        return (SEL *)ERROR_PTR("pix not defined", procName, NULL);
    if (cy < 0 || cx < 0)
        return (SEL *)ERROR_PTR("(cy, cx) not both >= 0", procName, NULL);
    pixGetDimensions(pix, &w, &h, &d);
    if (d != 1)
        return (SEL *)ERROR_PTR("pix not 1 bpp", procName, NULL);

    sel = selCreate(h, w, name);
    selSetOrigin(sel, cy, cx);
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            pixGetPixel(pix, j, i, &val);
            if (val)
                selSetElement(sel, i, j, SEL_HIT);
        }
    }

    return sel;
}
