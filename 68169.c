selCreateComb(l_int32  factor1,
              l_int32  factor2,
              l_int32  direction)
{
l_int32  i, size, z;
SEL     *sel;

    PROCNAME("selCreateComb");

    if (factor1 < 1 || factor2 < 1)
        return (SEL *)ERROR_PTR("factors must be >= 1", procName, NULL);
    if (direction != L_HORIZ && direction != L_VERT)
        return (SEL *)ERROR_PTR("invalid direction", procName, NULL);

    size = factor1 * factor2;
    if (direction == L_HORIZ) {
        sel = selCreate(1, size, NULL);
        selSetOrigin(sel, 0, size / 2);
    } else {
        sel = selCreate(size, 1, NULL);
        selSetOrigin(sel, size / 2, 0);
    }

        /* Lay down the elements of the comb */
    for (i = 0; i < factor2; i++) {
        z = factor1 / 2 + i * factor1;
/*        fprintf(stderr, "i = %d, factor1 = %d, factor2 = %d, z = %d\n",
                        i, factor1, factor2, z); */
        if (direction == L_HORIZ)
            selSetElement(sel, 0, z, SEL_HIT);
        else
            selSetElement(sel, z, 0, SEL_HIT);
    }

    return sel;
}
