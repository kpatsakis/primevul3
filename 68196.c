selaRead(const char  *fname)
{
FILE  *fp;
SELA  *sela;

    PROCNAME("selaRead");

    if (!fname)
        return (SELA *)ERROR_PTR("fname not defined", procName, NULL);

    if ((fp = fopenReadStream(fname)) == NULL)
        return (SELA *)ERROR_PTR("stream not opened", procName, NULL);
    if ((sela = selaReadStream(fp)) == NULL) {
        fclose(fp);
        return (SELA *)ERROR_PTR("sela not returned", procName, NULL);
    }
    fclose(fp);

    return sela;
}
