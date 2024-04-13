ptaaWriteStream(FILE    *fp,
                PTAA    *ptaa,
                l_int32  type)
{
l_int32  i, n;
PTA     *pta;

    PROCNAME("ptaaWriteStream");

    if (!fp)
        return ERROR_INT("stream not defined", procName, 1);
    if (!ptaa)
        return ERROR_INT("ptaa not defined", procName, 1);

    n = ptaaGetCount(ptaa);
    fprintf(fp, "\nPtaa Version %d\n", PTA_VERSION_NUMBER);
    fprintf(fp, "Number of Pta = %d\n", n);
    for (i = 0; i < n; i++) {
        pta = ptaaGetPta(ptaa, i, L_CLONE);
        ptaWriteStream(fp, pta, type);
        ptaDestroy(&pta);
    }

    return 0;
}
