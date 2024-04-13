selaExtendArray(SELA  *sela)
{
    PROCNAME("selaExtendArray");

    if (!sela)
        return ERROR_INT("sela not defined", procName, 1);

    if ((sela->sel = (SEL **)reallocNew((void **)&sela->sel,
                              sizeof(SEL *) * sela->nalloc,
                              2 * sizeof(SEL *) * sela->nalloc)) == NULL)
            return ERROR_INT("new ptr array not returned", procName, 1);

    sela->nalloc = 2 * sela->nalloc;
    return 0;
}
