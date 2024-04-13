selDisplayInPix(SEL     *sel,
                l_int32  size,
                l_int32  gthick)
{
l_int32  i, j, w, h, sx, sy, cx, cy, type, width;
l_int32  radius1, radius2, shift1, shift2, x0, y0;
PIX     *pixd, *pix2, *pixh, *pixm, *pixorig;
PTA     *pta1, *pta2, *pta1t, *pta2t;

    PROCNAME("selDisplayInPix");

    if (!sel)
        return (PIX *)ERROR_PTR("sel not defined", procName, NULL);
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
    selGetParameters(sel, &sy, &sx, &cy, &cx);
    w = size * sx + gthick * (sx + 1);
    h = size * sy + gthick * (sy + 1);
    pixd = pixCreate(w, h, 1);

        /* Generate grid lines */
    for (i = 0; i <= sy; i++)
        pixRenderLine(pixd, 0, gthick / 2 + i * (size + gthick),
                      w - 1, gthick / 2 + i * (size + gthick),
                      gthick, L_SET_PIXELS);
    for (j = 0; j <= sx; j++)
        pixRenderLine(pixd, gthick / 2 + j * (size + gthick), 0,
                      gthick / 2 + j * (size + gthick), h - 1,
                      gthick, L_SET_PIXELS);

        /* Generate hit and miss patterns */
    radius1 = (l_int32)(0.85 * ((size - 1) / 2.0) + 0.5);  /* of hit */
    radius2 = (l_int32)(0.65 * ((size - 1) / 2.0) + 0.5);  /* of inner miss */
    pta1 = generatePtaFilledCircle(radius1);
    pta2 = generatePtaFilledCircle(radius2);
    shift1 = (size - 1) / 2 - radius1;  /* center circle in square */
    shift2 = (size - 1) / 2 - radius2;
    pta1t = ptaTransform(pta1, shift1, shift1, 1.0, 1.0);
    pta2t = ptaTransform(pta2, shift2, shift2, 1.0, 1.0);
    pixh = pixGenerateFromPta(pta1t, size, size);  /* hits */
    pix2 = pixGenerateFromPta(pta2t, size, size);
    pixm = pixSubtract(NULL, pixh, pix2);

        /* Generate crossed lines for origin pattern */
    pixorig = pixCreate(size, size, 1);
    width = size / 8;
    pixRenderLine(pixorig, size / 2, (l_int32)(0.12 * size),
                           size / 2, (l_int32)(0.88 * size),
                           width, L_SET_PIXELS);
    pixRenderLine(pixorig, (l_int32)(0.15 * size), size / 2,
                           (l_int32)(0.85 * size), size / 2,
                           width, L_FLIP_PIXELS);
    pixRasterop(pixorig, size / 2 - width, size / 2 - width,
                2 * width, 2 * width, PIX_NOT(PIX_DST), NULL, 0, 0);

        /* Specialize origin pattern for this sel */
    selGetTypeAtOrigin(sel, &type);
    if (type == SEL_HIT)
        pixXor(pixorig, pixorig, pixh);
    else if (type == SEL_MISS)
        pixXor(pixorig, pixorig, pixm);

        /* Paste the patterns in */
    y0 = gthick;
    for (i = 0; i < sy; i++) {
        x0 = gthick;
        for (j = 0; j < sx; j++) {
            selGetElement(sel, i, j, &type);
            if (i == cy && j == cx)  /* origin */
                pixRasterop(pixd, x0, y0, size, size, PIX_SRC, pixorig, 0, 0);
            else if (type == SEL_HIT)
                pixRasterop(pixd, x0, y0, size, size, PIX_SRC, pixh, 0, 0);
            else if (type == SEL_MISS)
                pixRasterop(pixd, x0, y0, size, size, PIX_SRC, pixm, 0, 0);
            x0 += size + gthick;
        }
        y0 += size + gthick;
    }

    pixDestroy(&pix2);
    pixDestroy(&pixh);
    pixDestroy(&pixm);
    pixDestroy(&pixorig);
    ptaDestroy(&pta1);
    ptaDestroy(&pta1t);
    ptaDestroy(&pta2);
    ptaDestroy(&pta2t);
    return pixd;
}
