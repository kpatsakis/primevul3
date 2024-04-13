ptaaReadStream(FILE  *fp)
{
l_int32  i, n, version;
PTA     *pta;
PTAA    *ptaa;

    PROCNAME("ptaaReadStream");

    if (!fp)
        return (PTAA *)ERROR_PTR("stream not defined", procName, NULL);

    if (fscanf(fp, "\nPtaa Version %d\n", &version) != 1)
        return (PTAA *)ERROR_PTR("not a ptaa file", procName, NULL);
    if (version != PTA_VERSION_NUMBER)
        return (PTAA *)ERROR_PTR("invalid ptaa version", procName, NULL);
    if (fscanf(fp, "Number of Pta = %d\n", &n) != 1)
        return (PTAA *)ERROR_PTR("not a ptaa file", procName, NULL);

    if ((ptaa = ptaaCreate(n)) == NULL)
        return (PTAA *)ERROR_PTR("ptaa not made", procName, NULL);
    for (i = 0; i < n; i++) {
        if ((pta = ptaReadStream(fp)) == NULL) {
            ptaaDestroy(&ptaa);
            return (PTAA *)ERROR_PTR("error reading pta", procName, NULL);
        }
        ptaaAddPta(ptaa, pta, L_INSERT);
    }

    return ptaa;
}
