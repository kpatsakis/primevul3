ptaReadMem(const l_uint8  *data,
           size_t          size)
{
FILE  *fp;
PTA   *pta;

    PROCNAME("ptaReadMem");

    if (!data)
        return (PTA *)ERROR_PTR("data not defined", procName, NULL);
    if ((fp = fopenReadFromMemory(data, size)) == NULL)
        return (PTA *)ERROR_PTR("stream not opened", procName, NULL);

    pta = ptaReadStream(fp);
    fclose(fp);
    if (!pta) L_ERROR("pta not read\n", procName);
    return pta;
}
