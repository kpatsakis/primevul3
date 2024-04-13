selaWriteStream(FILE  *fp,
                SELA  *sela)
{
l_int32  i, n;
SEL     *sel;

    PROCNAME("selaWriteStream");

    if (!fp)
        return ERROR_INT("stream not defined", procName, 1);
    if (!sela)
        return ERROR_INT("sela not defined", procName, 1);

    n = selaGetCount(sela);
    fprintf(fp, "\nSela Version %d\n", SEL_VERSION_NUMBER);
    fprintf(fp, "Number of Sels = %d\n\n", n);
    for (i = 0; i < n; i++) {
        if ((sel = selaGetSel(sela, i)) == NULL)
            continue;
        selWriteStream(fp, sel);
    }
    return 0;
}
