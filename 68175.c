selDestroy(SEL  **psel)
{
l_int32  i;
SEL     *sel;

    PROCNAME("selDestroy");

    if (psel == NULL)  {
        L_WARNING("ptr address is NULL!\n", procName);
        return;
    }
    if ((sel = *psel) == NULL)
        return;

    for (i = 0; i < sel->sy; i++)
        LEPT_FREE(sel->data[i]);
    LEPT_FREE(sel->data);
    if (sel->name)
        LEPT_FREE(sel->name);
    LEPT_FREE(sel);

    *psel = NULL;
    return;
}
