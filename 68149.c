ptaWrite(const char  *filename,
         PTA         *pta,
         l_int32      type)
{
l_int32  ret;
FILE    *fp;

    PROCNAME("ptaWrite");

    if (!filename)
        return ERROR_INT("filename not defined", procName, 1);
    if (!pta)
        return ERROR_INT("pta not defined", procName, 1);

    if ((fp = fopenWriteStream(filename, "w")) == NULL)
        return ERROR_INT("stream not opened", procName, 1);
    ret = ptaWriteStream(fp, pta, type);
    fclose(fp);
    if (ret)
        return ERROR_INT("pta not written to stream", procName, 1);
    return 0;
}
