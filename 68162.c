ptaaWrite(const char  *filename,
          PTAA        *ptaa,
          l_int32      type)
{
l_int32  ret;
FILE    *fp;

    PROCNAME("ptaaWrite");

    if (!filename)
        return ERROR_INT("filename not defined", procName, 1);
    if (!ptaa)
        return ERROR_INT("ptaa not defined", procName, 1);

    if ((fp = fopenWriteStream(filename, "w")) == NULL)
        return ERROR_INT("stream not opened", procName, 1);
    ret = ptaaWriteStream(fp, ptaa, type);
    fclose(fp);
    if (ret)
        return ERROR_INT("ptaa not written to stream", procName, 1);
    return 0;
}
