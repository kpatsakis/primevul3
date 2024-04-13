selaDestroy(SELA  **psela)
{
SELA    *sela;
l_int32  i;

    if (!psela) return;
    if ((sela = *psela) == NULL)
        return;

    for (i = 0; i < sela->n; i++)
        selDestroy(&sela->sel[i]);
    LEPT_FREE(sela->sel);
    LEPT_FREE(sela);
    *psela = NULL;
    return;
}
