ptaaWriteMem(l_uint8  **pdata,
             size_t    *psize,
             PTAA      *ptaa,
             l_int32    type)
{
l_int32  ret;
FILE    *fp;

    PROCNAME("ptaaWriteMem");

    if (pdata) *pdata = NULL;
    if (psize) *psize = 0;
    if (!pdata)
        return ERROR_INT("&data not defined", procName, 1);
    if (!psize)
        return ERROR_INT("&size not defined", procName, 1);
    if (!ptaa)
        return ERROR_INT("ptaa not defined", procName, 1);

#if HAVE_FMEMOPEN
    if ((fp = open_memstream((char **)pdata, psize)) == NULL)
        return ERROR_INT("stream not opened", procName, 1);
    ret = ptaaWriteStream(fp, ptaa, type);
#else
    L_INFO("work-around: writing to a temp file\n", procName);
  #ifdef _WIN32
    if ((fp = fopenWriteWinTempfile()) == NULL)
        return ERROR_INT("tmpfile stream not opened", procName, 1);
  #else
    if ((fp = tmpfile()) == NULL)
        return ERROR_INT("tmpfile stream not opened", procName, 1);
  #endif  /* _WIN32 */
    ret = ptaaWriteStream(fp, ptaa, type);
    rewind(fp);
    *pdata = l_binaryReadStream(fp, psize);
#endif  /* HAVE_FMEMOPEN */
    fclose(fp);
    return ret;
}
