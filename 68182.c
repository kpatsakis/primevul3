selRead(const char  *fname)
{
FILE  *fp;
SEL   *sel;

    PROCNAME("selRead");

    if (!fname)
        return (SEL *)ERROR_PTR("fname not defined", procName, NULL);

    if ((fp = fopenReadStream(fname)) == NULL)
        return (SEL *)ERROR_PTR("stream not opened", procName, NULL);
    if ((sel = selReadStream(fp)) == NULL) {
        fclose(fp);
        return (SEL *)ERROR_PTR("sela not returned", procName, NULL);
    }
    fclose(fp);

    return sel;
}
