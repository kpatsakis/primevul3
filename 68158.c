ptaaReadMem(const l_uint8  *data,
            size_t          size)
{
FILE  *fp;
PTAA  *ptaa;

    PROCNAME("ptaaReadMem");

    if (!data)
        return (PTAA *)ERROR_PTR("data not defined", procName, NULL);
    if ((fp = fopenReadFromMemory(data, size)) == NULL)
        return (PTAA *)ERROR_PTR("stream not opened", procName, NULL);

    ptaa = ptaaReadStream(fp);
    fclose(fp);
    if (!ptaa) L_ERROR("ptaa not read\n", procName);
    return ptaa;
}
