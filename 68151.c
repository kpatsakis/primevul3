ptaWriteStream(FILE    *fp,
               PTA     *pta,
               l_int32  type)
{
l_int32    i, n, ix, iy;
l_float32  x, y;

    PROCNAME("ptaWriteStream");

    if (!fp)
        return ERROR_INT("stream not defined", procName, 1);
    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);

    n = ptaGetCount(pta);
    fprintf(fp, "\n Pta Version %d\n", PTA_VERSION_NUMBER);
    if (type == 0)
        fprintf(fp, " Number of pts = %d; format = float\n", n);
    else  /* type == 1 */
        fprintf(fp, " Number of pts = %d; format = integer\n", n);
    for (i = 0; i < n; i++) {
        if (type == 0) {  /* data is float */
            ptaGetPt(pta, i, &x, &y);
            fprintf(fp, "   (%f, %f)\n", x, y);
        } else {   /* data is integer */
            ptaGetIPt(pta, i, &ix, &iy);
            fprintf(fp, "   (%d, %d)\n", ix, iy);
        }
    }

    return 0;
}
