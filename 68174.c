selCreateFromString(const char  *text,
                    l_int32      h,
                    l_int32      w,
                    const char  *name)
{
SEL     *sel;
l_int32  y, x, norig;
char     ch;

    PROCNAME("selCreateFromString");

    if (h < 1)
        return (SEL *)ERROR_PTR("height must be > 0", procName, NULL);
    if (w < 1)
        return (SEL *)ERROR_PTR("width must be > 0", procName, NULL);

    sel = selCreate(h, w, name);

    norig = 0;
    for (y = 0; y < h; ++y) {
        for (x = 0; x < w; ++x) {
            ch = *(text++);
            switch (ch)
            {
                case 'X':
                    norig++;
                    selSetOrigin(sel, y, x);
                case 'x':
                    selSetElement(sel, y, x, SEL_HIT);
                    break;

                case 'O':
                    norig++;
                    selSetOrigin(sel, y, x);
                case 'o':
                    selSetElement(sel, y, x, SEL_MISS);
                    break;

                case 'C':
                    norig++;
                    selSetOrigin(sel, y, x);
                case ' ':
                    selSetElement(sel, y, x, SEL_DONT_CARE);
                    break;

                case '\n':
                    /* ignored */
                    continue;

                default:
                    selDestroy(&sel);
                    return (SEL *)ERROR_PTR("unknown char", procName, NULL);
            }
        }
    }
    if (norig != 1) {
        L_ERROR("Exactly one origin must be specified; this string has %d\n",
                procName, norig);
        selDestroy(&sel);
    }

    return sel;
}
