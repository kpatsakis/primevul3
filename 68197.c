selaReadStream(FILE  *fp)
{
l_int32  i, n, version;
SEL     *sel;
SELA    *sela;

    PROCNAME("selaReadStream");

    if (!fp)
        return (SELA *)ERROR_PTR("stream not defined", procName, NULL);

    if (fscanf(fp, "\nSela Version %d\n", &version) != 1)
        return (SELA *)ERROR_PTR("not a sela file", procName, NULL);
    if (version != SEL_VERSION_NUMBER)
        return (SELA *)ERROR_PTR("invalid sel version", procName, NULL);
    if (fscanf(fp, "Number of Sels = %d\n\n", &n) != 1)
        return (SELA *)ERROR_PTR("not a sela file", procName, NULL);

    if ((sela = selaCreate(n)) == NULL)
        return (SELA *)ERROR_PTR("sela not made", procName, NULL);
    sela->nalloc = n;

    for (i = 0; i < n; i++) {
        if ((sel = selReadStream(fp)) == NULL) {
            selaDestroy(&sela);
            return (SELA *)ERROR_PTR("sel not read", procName, NULL);
        }
        selaAddSel(sela, sel, NULL, 0);
    }

    return sela;
}
