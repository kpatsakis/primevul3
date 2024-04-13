ptaRead(const char  *filename)
{
FILE  *fp;
PTA   *pta;

    PROCNAME("ptaRead");

    if (!filename)
        return (PTA *)ERROR_PTR("filename not defined", procName, NULL);

    if ((fp = fopenReadStream(filename)) == NULL)
        return (PTA *)ERROR_PTR("stream not opened", procName, NULL);
    pta = ptaReadStream(fp);
    fclose(fp);
    if (!pta)
        return (PTA *)ERROR_PTR("pta not read", procName, NULL);
    return pta;
}
