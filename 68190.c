selaDisplayInPix(SELA    *sela,
                 l_int32  size,
                 l_int32  gthick,
                 l_int32  spacing,
                 l_int32  ncols)
{
l_int32  nsels, i, w, width;
PIX     *pixt, *pixd;
PIXA    *pixa;
SEL     *sel;

    PROCNAME("selaDisplayInPix");

    if (!sela)
        return (PIX *)ERROR_PTR("sela not defined", procName, NULL);
    if (size < 13) {
        L_WARNING("size < 13; setting to 13\n", procName);
        size = 13;
    }
    if (size % 2 == 0)
        size++;
    if (gthick < 2) {
        L_WARNING("grid thickness < 2; setting to 2\n", procName);
        gthick = 2;
    }
    if (spacing < 5) {
        L_WARNING("spacing < 5; setting to 5\n", procName);
        spacing = 5;
    }

        /* Accumulate the pix of each sel */
    nsels = selaGetCount(sela);
    pixa = pixaCreate(nsels);
    for (i = 0; i < nsels; i++) {
        sel = selaGetSel(sela, i);
        pixt = selDisplayInPix(sel, size, gthick);
        pixaAddPix(pixa, pixt, L_INSERT);
    }

        /* Find the tiled output width, using just the first
         * ncols pix in the pixa.   If all pix have the same width,
         * they will align properly in columns. */
    width = 0;
    ncols = L_MIN(nsels, ncols);
    for (i = 0; i < ncols; i++) {
        pixt = pixaGetPix(pixa, i, L_CLONE);
        pixGetDimensions(pixt, &w, NULL, NULL);
        width += w;
        pixDestroy(&pixt);
    }
    width += (ncols + 1) * spacing;  /* add spacing all around as well */

    pixd = pixaDisplayTiledInRows(pixa, 1, width, 1.0, 0, spacing, 0);
    pixaDestroy(&pixa);
    return pixd;
}
