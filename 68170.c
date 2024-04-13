selCreateFromColorPix(PIX   *pixs,
                      char  *selname)
{
PIXCMAP  *cmap;
SEL      *sel;
l_int32   hascolor, hasorigin, nohits;
l_int32   w, h, d, i, j, red, green, blue;
l_uint32  pixval;

    PROCNAME("selCreateFromColorPix");

    if (!pixs)
        return (SEL *)ERROR_PTR("pixs not defined", procName, NULL);

    hascolor = FALSE;
    cmap = pixGetColormap(pixs);
    if (cmap)
        pixcmapHasColor(cmap, &hascolor);
    pixGetDimensions(pixs, &w, &h, &d);
    if (hascolor == FALSE && d != 32)
        return (SEL *)ERROR_PTR("pixs has no color", procName, NULL);

    if ((sel = selCreate (h, w, NULL)) == NULL)
        return (SEL *)ERROR_PTR ("sel not made", procName, NULL);
    selSetOrigin (sel, h / 2, w / 2);
    selSetName(sel, selname);

    hasorigin = FALSE;
    nohits = TRUE;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            pixGetPixel (pixs, j, i, &pixval);

            if (cmap) {
                pixcmapGetColor (cmap, pixval, &red, &green, &blue);
            } else {
                red = GET_DATA_BYTE (&pixval, COLOR_RED);
                green = GET_DATA_BYTE (&pixval, COLOR_GREEN);
                blue = GET_DATA_BYTE (&pixval, COLOR_BLUE);
            }

            if (red < 255 && green < 255 && blue < 255) {
                if (hasorigin)
                    L_WARNING("multiple origins in sel image\n", procName);
                selSetOrigin (sel, i, j);
                hasorigin = TRUE;
            }
            if (!red && green && !blue) {
                nohits = FALSE;
                selSetElement (sel, i, j, SEL_HIT);
            } else if (red && !green && !blue) {
                selSetElement (sel, i, j, SEL_MISS);
            } else if (red && green && blue) {
                selSetElement (sel, i, j, SEL_DONT_CARE);
            } else {
                selDestroy(&sel);
                return (SEL *)ERROR_PTR("invalid color", procName, NULL);
            }
        }
    }

    if (nohits) {
        selDestroy(&sel);
        return (SEL *)ERROR_PTR("no hits in sel", procName, NULL);
    }
    return sel;
}
