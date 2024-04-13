selaWrite(const char  *fname,
          SELA        *sela)
{
FILE  *fp;

    PROCNAME("selaWrite");

    if (!fname)
        return ERROR_INT("fname not defined", procName, 1);
    if (!sela)
        return ERROR_INT("sela not defined", procName, 1);

    if ((fp = fopenWriteStream(fname, "wb")) == NULL)
        return ERROR_INT("stream not opened", procName, 1);
    selaWriteStream(fp, sela);
    fclose(fp);

    return 0;
}
