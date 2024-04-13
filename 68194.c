selaGetCount(SELA  *sela)
{
    PROCNAME("selaGetCount");

    if (!sela)
        return ERROR_INT("sela not defined", procName, 0);

    return sela->n;
}
